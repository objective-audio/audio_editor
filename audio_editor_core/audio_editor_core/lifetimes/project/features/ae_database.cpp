//
//  ae_database.cpp
//

#include "ae_database.h"

#include <audio_editor_core/ae_db_constants.h>
#include <audio_editor_core/ae_db_module.h>
#include <audio_editor_core/ae_db_utils.h>
#include <cpp_utils/yas_assertion.h>
#include <cpp_utils/yas_thread.h>
#include <db/yas_db_umbrella.h>

#include <audio_editor_core/ae_file_ref.hpp>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<database> database::make_shared(std::filesystem::path const &db_file_path) {
    return make_shared(db::manager::make_shared(db_file_path, database_utils::make_model()));
}

std::shared_ptr<database> database::make_shared(std::shared_ptr<db::manager> const &manager) {
    auto db = std::shared_ptr<database>(new database{manager});
    db->_setup();
    return db;
}

database::database(std::shared_ptr<db::manager> const &manager)
    : _manager(manager), _event_notifier(observing::notifier<database_event>::make_shared()) {
}

db_modules_map const &database::modules() const {
    return this->_modules;
}

db_file_refs_map const &database::file_refs() const {
    return this->_file_refs;
}

db_markers_map const &database::markers() const {
    return this->_markers;
}

std::optional<db_edge> const &database::edge() const {
    return this->_edge;
}

bool database::is_processing() const {
    return this->_processing_count > 0;
}

db_module database::add_module(file_module const &args) {
    auto module = db_module::create(this->_manager, args);
    this->_modules.emplace(args.range, module);
    this->_save();
    return module;
}

void database::remove_module(time::range const &range) {
    if (this->_modules.contains(range)) {
        this->_modules.at(range).remove();
        this->_modules.erase(range);
        this->_save();
    }
}

void database::update_module(time::range const &range, file_module const &file_module) {
    if (this->_modules.contains(range)) {
        auto &db_module = this->_modules.at(range);
        db_module.set_name(file_module.name);
        db_module.set_range(file_module.range);
        this->_modules.erase(range);
        this->_modules.emplace(file_module.range, db_module);
        this->_save();
    }
}

void database::update_module_detail(file_module const &file_module) {
    auto const &range = file_module.range;

    if (this->_modules.contains(range)) {
        auto &db_module = this->_modules.at(range);
        db_module.set_name(file_module.name);
        this->_save();
    }
}

db_marker database::add_marker(frame_index_t const frame, std::string const &name) {
    auto marker = db_marker::create(this->_manager, frame, name);
    this->_markers.emplace(frame, marker);
    this->_save();
    return marker;
}

void database::remove_marker(frame_index_t const &frame) {
    if (this->_markers.contains(frame)) {
        this->_markers.at(frame).remove();
        this->_markers.erase(frame);
        this->_save();
    }
}

void database::update_marker(frame_index_t const &prev_frame, marker_object const &marker) {
    if (this->_markers.contains(prev_frame)) {
        auto &db_marker = this->_markers.at(prev_frame);
        db_marker.set_marker(marker);
        this->_markers.erase(prev_frame);
        this->_markers.emplace(marker.value.frame, db_marker);
        this->_save();
    }
}

void database::set_edge(ae::edge const &edge) {
    if (this->_edge.has_value()) {
        this->_edge.value().set(edge);
    } else {
        this->_edge.emplace(db_edge::create(this->_manager, edge));
    }

    this->_save();
}

std::optional<db_file_ref> database::add_file_ref(file_ref const &file_ref) {
    if (!this->_file_refs.contains(file_ref.file_name)) {
        auto db_file_ref = db_file_ref::create(this->_manager, file_ref);
        this->_file_refs.emplace(file_ref.file_name, db_file_ref);
        this->_save();
        return db_file_ref;
    } else {
        // 同じファイルを追加した場合は新規で作らずにスルーする
        return std::nullopt;
    }
}

void database::suspend_saving(std::function<void(void)> &&handler) {
    this->_save_caller.push();
    handler();
    this->_save_caller.pop();
}

void database::_save() {
    this->_save_caller.request([this] {
        this->_increment_processing_count();

        this->_manager->save(db::no_cancellation,
                             [weak_db = this->weak_from_this()](db::manager_map_result_t result) mutable {
                                 if (!result) {
                                     assertion_failure_if_not_test();
                                 }
                                 if (auto db = weak_db.lock()) {
                                     db->_decrement_processing_count();
                                 }
                             });
    });
}

bool database::can_undo() const {
    if (this->is_processing()) {
        return false;
    }

    return this->_current_save_id() > 1;
}

void database::undo() {
    if (!this->can_undo()) {
        return;
    }

    this->_revert(this->_current_save_id() - 1, false);
}

bool database::can_redo() const {
    if (this->is_processing()) {
        return false;
    }

    return this->_current_save_id() < this->_last_save_id();
}

void database::redo() {
    if (!this->can_redo()) {
        return;
    }

    this->_revert(this->_current_save_id() + 1, false);
}

bool database::can_purge() const {
    if (this->is_processing()) {
        return false;
    }

    return this->_last_save_id() > 1;
}

void database::purge() {
    if (!this->can_purge()) {
        return;
    }

    this->_increment_processing_count();

    this->_manager->purge(db::no_cancellation, [weak_db = this->weak_from_this()](db::manager_result_t const &result) {
        assert(thread::is_main());

        if (!result) {
            assertion_failure_if_not_test();
        }

        auto const database = weak_db.lock();
        if (!database) {
            return;
        }

        database->_decrement_processing_count();
        database->_event_notifier->notify(database_event::purged);
    });
}

observing::endable database::observe(std::function<void(database_event const &)> &&handler) {
    return this->_event_notifier->observe(std::move(handler));
}

void database::_setup() {
    this->_increment_processing_count();

    this->_manager->setup([weak_db = this->weak_from_this()](db::manager_result_t result) mutable {
        assert(thread::is_main());

        if (!result) {
            assertion_failure_if_not_test();
        }

        if (auto const database = weak_db.lock()) {
            database->_decrement_processing_count();

            database->_revert(database->_current_save_id(), true);
        }
    });
}

void database::_increment_processing_count() {
    this->_processing_count++;
}

void database::_decrement_processing_count() {
    this->_processing_count--;
}

db::integer::type const &database::_current_save_id() const {
    return this->_manager->current_save_id().get<db::integer>();
}

db::integer::type const &database::_last_save_id() const {
    return this->_manager->last_save_id().get<db::integer>();
}

void database::_revert(db::integer::type const revert_id, bool const is_initial) {
    if (this->_last_save_id() < revert_id || (this->_current_save_id() == revert_id && !is_initial)) {
        assertion_failure_if_not_test();
        return;
    }

    this->_increment_processing_count();

    this->_manager->suspend();

    this->_manager->revert(
        db::no_cancellation, [revert_id]() { return revert_id; }, [](db::manager_vector_result_t result) {});

    this->_manager->fetch_objects(
        db::no_cancellation,
        [] {
            return db::to_fetch_option(
                db::select_option{.table = db_constants::module_name::entity,
                                  .field_orders = {{db::object_id_field, db::order::ascending}}});
        },
        [weak_db = this->weak_from_this()](db::manager_vector_result_t result) {
            assert(thread::is_main());

            auto const database = weak_db.lock();
            if (database && result) {
                auto const &result_objects = result.value();

                db_modules_map modules;

                if (result_objects.contains(db_constants::module_name::entity)) {
                    auto const &objects = result_objects.at(db_constants::module_name::entity);
                    for (auto const &object : objects) {
                        db_module module{object};
                        if (auto const file_module = module.file_module()) {
                            modules.emplace(file_module.value().value.range, std::move(module));
                        }
                    }
                }

                database->_modules = std::move(modules);
            } else {
                assertion_failure_if_not_test();
            }
        });

    this->_manager->fetch_objects(
        db::no_cancellation,
        [] {
            return db::to_fetch_option(
                db::select_option{.table = db_constants::file_ref_name::entity,
                                  .field_orders = {{db::object_id_field, db::order::ascending}}});
        },
        [weak_db = this->weak_from_this()](db::manager_vector_result_t result) {
            assert(thread::is_main());

            auto const database = weak_db.lock();
            if (database && result) {
                auto const &result_objects = result.value();

                db_file_refs_map file_refs;

                if (result_objects.contains(db_constants::file_ref_name::entity)) {
                    auto const &objects = result_objects.at(db_constants::file_ref_name::entity);
                    for (auto const &object : objects) {
                        db_file_ref const db_ref{object};
                        if (auto const ref = db_ref.file_ref()) {
                            file_refs.emplace(ref.value().value.file_name, std::move(db_ref));
                        }
                    }
                }

                database->_file_refs = std::move(file_refs);
            } else {
                assertion_failure_if_not_test();
            }
        });

    this->_manager->fetch_objects(
        db::no_cancellation,
        [] {
            return db::to_fetch_option(
                db::select_option{.table = db_constants::marker_name::entity,
                                  .field_orders = {{db::object_id_field, db::order::ascending}}});
        },
        [weak_db = this->weak_from_this()](db::manager_vector_result_t result) {
            assert(thread::is_main());

            auto const database = weak_db.lock();
            if (database && result) {
                auto const &result_objects = result.value();

                db_markers_map markers;

                if (result_objects.contains(db_constants::marker_name::entity)) {
                    auto const &objects = result_objects.at(db_constants::marker_name::entity);
                    for (auto const &object : objects) {
                        db_marker db_marker{object};
                        if (auto const marker = db_marker.marker()) {
                            markers.emplace(marker.value().value.frame, std::move(db_marker));
                        }
                    }
                }

                database->_markers = std::move(markers);
            } else {
                assertion_failure_if_not_test();
            }
        });

    this->_manager->fetch_objects(
        db::no_cancellation,
        [] {
            return db::to_fetch_option(
                db::select_option{.table = db_constants::edge_name::entity,
                                  .field_orders = {{db::object_id_field, db::order::ascending}}});
        },
        [weak_db = this->weak_from_this()](db::manager_vector_result_t result) {
            assert(thread::is_main());

            auto const database = weak_db.lock();
            if (database && result) {
                auto const &result_objects = result.value();

                database->_edge.reset();

                if (result_objects.contains(db_constants::edge_name::entity)) {
                    auto const &objects = result_objects.at(db_constants::edge_name::entity);
                    if (!objects.empty()) {
                        database->_edge.emplace(objects.at(0));
                    }
                }
            } else {
                assertion_failure_if_not_test();
            }
        });

    this->_manager->execute(db::no_cancellation, [weak_db = this->weak_from_this()](auto const &) {
        assert(!thread::is_main());

        thread::perform_sync_on_main([weak_db] {
            auto const database = weak_db.lock();
            if (database) {
                if (!database->edge().has_value()) {
                    database->set_edge(ae::edge::zero());
                    database->_save();
                }

                database->_decrement_processing_count();
                database->_event_notifier->notify(database_event::reverted);
            } else {
                assertion_failure_if_not_test();
            }
        });
    });

    this->_manager->resume();
}

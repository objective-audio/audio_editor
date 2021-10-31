//
//  ae_database.cpp
//

#include "ae_database.h"

#include <audio_editor_core/ae_db_constants.h>
#include <audio_editor_core/ae_db_module.h>
#include <audio_editor_core/ae_db_utils.h>
#include <cpp_utils/yas_thread.h>
#include <cpp_utils/yas_url.h>
#include <db/yas_db_umbrella.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<database> database::make_shared(url const &db_file_url) {
    return make_shared(db::manager::make_shared(db_file_url.path(), database_utils::make_model()));
}

std::shared_ptr<database> database::make_shared(std::shared_ptr<db::manager> const &manager) {
    auto db = std::shared_ptr<database>(new database{manager});
    db->_setup(db);
    return db;
}

database::database(std::shared_ptr<db::manager> const &manager)
    : _manager(manager), _reverted_notifier(observing::notifier<db_modules_map>::make_shared()) {
}

bool database::is_processing() const {
    return this->_processing_count > 0;
}

void database::add_module(file_module const &file_module) {
    this->_modules.emplace(file_module.range, db_module::create(this->_manager, file_module));
}

void database::remove_module(proc::time::range const &range) {
    if (this->_modules.count(range)) {
        this->_modules.at(range).remove();
        this->_modules.erase(range);
    }
}

void database::save() {
    this->_increment_processing_count();

    this->_manager->save(db::no_cancellation,
                         [weak_db = this->_weak_database](db::manager_map_result_t result) mutable {
                             if (auto db = weak_db.lock()) {
                                 db->_decrement_processing_count();
                             }
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

    this->_revert(this->_current_save_id() - 1);
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

    this->_revert(this->_current_save_id() + 1);
}

observing::endable database::observe_reverted(std::function<void(db_modules_map const &)> &&handler) {
    return this->_reverted_notifier->observe(std::move(handler));
}

void database::_setup(std::weak_ptr<database> weak_db) {
    this->_weak_database = std::move(weak_db);

    this->_increment_processing_count();

    this->_manager->setup([weak_db = this->_weak_database](db::manager_result_t result) mutable {
        if (auto database = weak_db.lock()) {
            database->_decrement_processing_count();
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

void database::_revert(db::integer::type const revert_id) {
    if (this->_last_save_id() < revert_id || this->_current_save_id() == revert_id) {
        assert(0);
        return;
    }

    this->_increment_processing_count();

    this->_manager->revert(
        db::no_cancellation, [revert_id]() { return revert_id; }, [](db::manager_vector_result_t result) {});

    this->_manager->fetch_objects(
        db::no_cancellation,
        [] {
            return db::to_fetch_option(
                db::select_option{.table = db_constants::module::entity_name,
                                  .field_orders = {{db::object_id_field, db::order::ascending}}});
        },
        [weak_db = this->_weak_database](db::manager_vector_result_t result) mutable {
            auto const database = weak_db.lock();
            if (database) {
                if (result) {
                    auto const &result_objects = result.value();

                    db_modules_map modules;

                    if (result_objects.contains(db_constants::module::entity_name)) {
                        auto const &objects = result_objects.at(db_constants::module::entity_name);
                        for (auto const &object : objects) {
                            db_module module{object};
                            if (auto const file_module = module.file_module()) {
                                modules.emplace(file_module.value().range, std::move(module));
                            }
                        }
                    }

                    database->_modules = std::move(modules);
                    database->_reverted_notifier->notify(database->_modules);
                }

                database->_decrement_processing_count();
            }
        });
}

//
//  ae_module_pool.cpp
//

#include "ae_module_pool.h"

#include <audio_editor_core/ae_db_module.h>
#include <audio_editor_core/ae_module_pool_utils.h>
#include <cpp_utils/yas_assertion.h>
#include <cpp_utils/yas_stl_utils.h>

using namespace yas;
using namespace yas::ae;

module_pool::module_pool(database_for_module_pool *database) : _database(database) {
    this->_event_fetcher = observing::fetcher<module_pool_event>::make_shared(
        [this] { return module_pool_event{.type = module_pool_event_type::any, .modules = this->_modules}; });
}

module_pool_module_map_t const &module_pool::modules() const {
    return this->_modules;
}

void module_pool::revert_modules_and_notify(std::vector<module_object> &&modules) {
    this->_modules = to_map<module_index>(modules, [](module_object const &module) { return module.index(); });
    this->_event_fetcher->push({.type = module_pool_event_type::reverted, .modules = this->_modules});
}

std::optional<module_index> module_pool::insert_module_and_notify(module const &params) {
    auto db_module = this->_database->add_module(params);

    if (auto const object = db_module.object(); object.has_value()) {
        auto const &module = object.value();
        this->_modules.emplace(module.index(), module);
        this->_event_fetcher->push(
            {.type = module_pool_event_type::inserted, .module = module, .modules = this->_modules});
        return object->index();
    }

    return std::nullopt;
}

void module_pool::erase_module_and_notify(module_index const &index) {
    if (this->_modules.contains(index)) {
        auto module = this->_modules.at(index);
        this->_modules.erase(index);
        this->_database->remove_module(index.object_id);
        this->_event_fetcher->push(
            {.type = module_pool_event_type::erased, .module = module, .modules = this->_modules});
    }
}

void module_pool::set_module_name_and_notify(module_index const &index, std::string const &name) {
    if (this->_modules.contains(index)) {
        auto &module = this->_modules.at(index);
        if (module.value.name != name) {
            module.value.name = name;
            this->_database->update_module(index.object_id, module.value);
            this->_event_fetcher->push(
                {.type = module_pool_event_type::detail_updated, .module = module, .modules = this->_modules});
        }
    }
}

std::optional<time::range> module_pool::total_range() const {
    return module_pool_utils::total_range(this->_modules);
}

std::optional<module_object> module_pool::module_at(module_index const &index) const {
    if (this->_modules.contains(index)) {
        return this->_modules.at(index);
    } else {
        return std::nullopt;
    }
}

std::optional<module_object> module_pool::module_at(frame_index_t const frame) const {
    return module_pool_utils::module(this->_modules, frame);
}

std::optional<module_object> module_pool::previous_module_at(frame_index_t const frame) const {
    return module_pool_utils::previous_module(this->_modules, frame);
}

std::optional<module_object> module_pool::next_module_at(frame_index_t const frame) const {
    return module_pool_utils::next_module(this->_modules, frame);
}

std::optional<module_object> module_pool::splittable_module_at(frame_index_t const frame) const {
    return module_pool_utils::splittable_module(this->_modules, frame);
}

std::optional<module_object> module_pool::first_module() const {
    return module_pool_utils::first_module(this->_modules);
}

std::optional<module_object> module_pool::last_module() const {
    return module_pool_utils::last_module(this->_modules);
}

std::optional<frame_index_t> module_pool::next_jumpable_frame(frame_index_t const frame) const {
    if (auto const &module = this->module_at(frame); module.has_value()) {
        return module->value.range.next_frame();
    }

    if (auto const &module = this->next_module_at(frame); module.has_value()) {
        return module->value.range.frame;
    }

    return std::nullopt;
}

std::optional<frame_index_t> module_pool::previous_jumpable_frame(frame_index_t const frame) const {
    if (auto const &module = this->module_at(frame); module.has_value()) {
        if (module->value.range.frame < frame) {
            return module->value.range.frame;
        }
    }

    if (auto const &module = this->previous_module_at(frame)) {
        if (module->value.range.next_frame() == frame) {
            return module->value.range.frame;
        } else {
            return module->value.range.next_frame();
        }
    }

    return std::nullopt;
}

void module_pool::split_at(frame_index_t const frame) {
    if (auto const module_opt = this->splittable_module_at(frame); module_opt.has_value()) {
        auto const &module = module_opt.value().value;
        this->erase_module_and_notify(module_opt.value().index());
        this->insert_module_and_notify(module.tail_dropped(frame).value());
        this->insert_module_and_notify(module.head_dropped(frame).value());
    }
}

void module_pool::erase_at(frame_index_t const frame) {
    if (auto const module_opt = this->module_at(frame); module_opt.has_value()) {
        this->erase_module_and_notify(module_opt.value().index());
    }
}

void module_pool::erase_and_offset_at(frame_index_t const frame) {
    if (auto const module_opt = this->module_at(frame); module_opt.has_value()) {
        auto const &module = module_opt.value().value;
        frame_index_t const offset = -int64_t(module.range.length);

        this->erase_module_and_notify(module_opt.value().index());
        this->_move_modules_after(frame, offset);
    }
}

void module_pool::drop_head_at(frame_index_t const frame) {
    if (auto const module_opt = this->splittable_module_at(frame); module_opt.has_value()) {
        this->erase_module_and_notify(module_opt.value().index());
        this->insert_module_and_notify(module_opt.value().value.head_dropped(frame).value());
    }
}

void module_pool::drop_tail_at(frame_index_t const frame) {
    if (auto const module_opt = this->splittable_module_at(frame); module_opt.has_value()) {
        this->erase_module_and_notify(module_opt.value().index());
        this->insert_module_and_notify(module_opt.value().value.tail_dropped(frame).value());
    }
}

void module_pool::drop_head_and_offset_at(frame_index_t const frame) {
    if (auto const module_opt = this->splittable_module_at(frame); module_opt.has_value()) {
        auto const &module = module_opt.value().value;
        frame_index_t const offset = module.range.frame - frame;

        this->erase_module_and_notify(module_opt.value().index());
        this->insert_module_and_notify(module.head_dropped(frame).value());
        this->_move_modules_after(frame, offset);
    }
}

void module_pool::drop_tail_and_offset_at(frame_index_t const frame) {
    if (auto const module_opt = this->splittable_module_at(frame); module_opt.has_value()) {
        auto const &module = module_opt.value().value;
        frame_index_t const offset = frame - module.range.next_frame();

        this->erase_module_and_notify(module_opt.value().index());
        this->insert_module_and_notify(module.tail_dropped(frame).value());

        this->_move_modules_after(frame, offset);
    }
}

void module_pool::overwrite_module(module const &params) {
    auto const overlapped_modules = module_pool_utils::overlapped_modules(this->_modules, params.range);
    for (auto const &overlapped_module : overlapped_modules) {
        this->erase_module_and_notify(overlapped_module.index());
        auto const cropped_ranges = overlapped_module.value.range.cropped(params.range);
        for (auto const &cropped_range : cropped_ranges) {
            frame_index_t const file_frame_offset = cropped_range.frame - overlapped_module.value.range.frame;
            this->insert_module_and_notify({overlapped_module.value.name, cropped_range, overlapped_module.value.track,
                                            overlapped_module.value.file_frame + file_frame_offset,
                                            overlapped_module.value.file_name});
        }
    }

    this->insert_module_and_notify(params);
}

void module_pool::move_modules(std::set<module_index> const &indices, frame_index_t const offset) {
    std::vector<module> movings;
    for (auto const &index : indices) {
        if (this->_modules.contains(index)) {
            auto const &module = this->_modules.at(index);
            movings.emplace_back(module.value.offset(offset));
            this->erase_module_and_notify(index);
        } else {
            assertion_failure_if_not_test();
        }
    }

    for (auto const &params : movings) {
        this->overwrite_module(params);
    }
}

void module_pool::split_and_insert_module_and_offset(module const &params) {
    this->split_at(params.range.frame);

    this->_move_modules_after(params.range.frame, params.range.length);
    this->insert_module_and_notify(params);
}

void module_pool::_move_modules_after(frame_index_t const frame, frame_index_t const offset) {
    auto const copied_modules = this->_modules;
    for (auto const &pair : copied_modules) {
        auto const &index = pair.first;
        if (frame <= index.range.frame) {
            // TODO 同じidのまま移動させたい？
            auto const &moving_module = pair.second.value;
            this->erase_module_and_notify(index);
            this->insert_module_and_notify(moving_module.offset(offset));
        }
    }
}

observing::syncable module_pool::observe_event(std::function<void(module_pool_event const &)> &&handler) {
    return this->_event_fetcher->observe(std::move(handler));
}

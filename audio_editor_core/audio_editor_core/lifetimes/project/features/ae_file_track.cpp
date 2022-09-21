//
//  ae_file_track.cpp
//

#include "ae_file_track.h"

#include <audio_editor_core/ae_db_module.h>
#include <audio_editor_core/ae_file_track_utils.h>
#include <cpp_utils/yas_stl_utils.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<file_track> file_track::make_shared(database_for_file_track *database) {
    return std::make_shared<file_track>(database);
}

file_track::file_track(database_for_file_track *database) : _database(database) {
    this->_event_fetcher = observing::fetcher<file_track_event>::make_shared(
        [this] { return file_track_event{.type = file_track_event_type::any, .modules = this->_modules}; });
}

file_track_module_map_t const &file_track::modules() const {
    return this->_modules;
}

void file_track::revert_modules_and_notify(std::vector<file_module_object> &&modules) {
    this->_modules = to_map<time::range>(modules, [](file_module_object const &module) { return module.value.range; });
    this->_event_fetcher->push({.type = file_track_event_type::reverted, .modules = this->_modules});
}

std::optional<object_id> file_track::insert_module_and_notify(file_module const &params) {
    auto db_module = this->_database->add_module(params);

    if (auto const file_module = db_module.file_module(); file_module.has_value()) {
        auto const &file_module_value = file_module.value();
        this->_modules.emplace(file_module_value.value.range, file_module_value);
        this->_event_fetcher->push(
            {.type = file_track_event_type::inserted, .module = file_module_value, .modules = this->_modules});
        return file_module->identifier;
    }

    return std::nullopt;
}

void file_track::erase_module_and_notify(time::range const &range) {
    if (this->_modules.contains(range)) {
        auto module = this->_modules.at(range);
        this->_modules.erase(range);
        this->_database->remove_module(range);
        this->_event_fetcher->push(
            {.type = file_track_event_type::erased, .module = module, .modules = this->_modules});
    }
}

void file_track::set_module_name_and_notify(time::range const &range, std::string const &name) {
    if (this->_modules.contains(range)) {
        auto &module = this->_modules.at(range);
        if (module.value.name != name) {
            module.value.name = name;
            this->_database->update_module(range, module.value);
            this->_event_fetcher->push(
                {.type = file_track_event_type::detail_updated, .module = module, .modules = this->_modules});
        }
    }
}

std::optional<time::range> file_track::total_range() const {
    return file_track_utils::total_range(this->_modules);
}

std::optional<file_module_object> file_track::module_at(frame_index_t const frame) const {
    return file_track_utils::module(this->_modules, frame);
}

std::optional<file_module_object> file_track::previous_module_at(frame_index_t const frame) const {
    return file_track_utils::previous_module(this->_modules, frame);
}

std::optional<file_module_object> file_track::next_module_at(frame_index_t const frame) const {
    return file_track_utils::next_module(this->_modules, frame);
}

std::optional<file_module_object> file_track::splittable_module_at(frame_index_t const frame) const {
    return file_track_utils::splittable_module(this->_modules, frame);
}

std::optional<file_module_object> file_track::first_module() const {
    return file_track_utils::first_module(this->_modules);
}

std::optional<file_module_object> file_track::last_module() const {
    return file_track_utils::last_module(this->_modules);
}

std::optional<frame_index_t> file_track::next_jumpable_frame(frame_index_t const frame) const {
    if (auto const &module = this->module_at(frame); module.has_value()) {
        return module->value.range.next_frame();
    }

    if (auto const &module = this->next_module_at(frame); module.has_value()) {
        return module->value.range.frame;
    }

    return std::nullopt;
}

std::optional<frame_index_t> file_track::previous_jumpable_frame(frame_index_t const frame) const {
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

void file_track::split_at(frame_index_t const frame) {
    if (auto const module_opt = this->splittable_module_at(frame); module_opt.has_value()) {
        auto const &module = module_opt.value().value;
        this->erase_module_and_notify(module.range);
        this->insert_module_and_notify(module.tail_dropped(frame).value());
        this->insert_module_and_notify(module.head_dropped(frame).value());
    }
}

void file_track::erase_at(frame_index_t const frame) {
    if (auto const module_opt = this->module_at(frame); module_opt.has_value()) {
        this->erase_module_and_notify(module_opt.value().value.range);
    }
}

void file_track::erase_and_offset_at(frame_index_t const frame) {
    if (auto const module_opt = this->module_at(frame); module_opt.has_value()) {
        auto const &module = module_opt.value().value;
        frame_index_t const offset = -int64_t(module.range.length);

        this->erase_module_and_notify(module.range);
        this->_move_modules_after(frame, offset);
    }
}

void file_track::drop_head_at(frame_index_t const frame) {
    if (auto const module_opt = this->splittable_module_at(frame); module_opt.has_value()) {
        auto const &module = module_opt.value().value;
        this->erase_module_and_notify(module.range);
        this->insert_module_and_notify(module.head_dropped(frame).value());
    }
}

void file_track::drop_tail_at(frame_index_t const frame) {
    if (auto const module_opt = this->splittable_module_at(frame); module_opt.has_value()) {
        auto const &module = module_opt.value().value;
        this->erase_module_and_notify(module.range);
        this->insert_module_and_notify(module.tail_dropped(frame).value());
    }
}

void file_track::drop_head_and_offset_at(frame_index_t const frame) {
    if (auto const module_opt = this->splittable_module_at(frame); module_opt.has_value()) {
        auto const &module = module_opt.value().value;
        frame_index_t const offset = module.range.frame - frame;

        this->erase_module_and_notify(module.range);
        this->insert_module_and_notify(module.head_dropped(frame).value());
        this->_move_modules_after(frame, offset);
    }
}

void file_track::drop_tail_and_offset_at(frame_index_t const frame) {
    if (auto const module_opt = this->splittable_module_at(frame); module_opt.has_value()) {
        auto const &module = module_opt.value().value;
        frame_index_t const offset = frame - module.range.next_frame();

        this->erase_module_and_notify(module.range);
        this->insert_module_and_notify(module.tail_dropped(frame).value());

        this->_move_modules_after(frame, offset);
    }
}

void file_track::overwrite_module(file_module const &params) {
    auto const overlapped_modules = file_track_utils::overlapped_modules(this->_modules, params.range);
    for (auto const &overlapped_module : overlapped_modules) {
        this->erase_module_and_notify(overlapped_module.value.range);
        auto const cropped_ranges = overlapped_module.value.range.cropped(params.range);
        for (auto const &cropped_range : cropped_ranges) {
            frame_index_t const file_frame_offset = cropped_range.frame - overlapped_module.value.range.frame;
            this->insert_module_and_notify({overlapped_module.value.name, cropped_range,
                                            overlapped_module.value.file_frame + file_frame_offset,
                                            overlapped_module.value.file_name});
        }
    }

    this->insert_module_and_notify(params);
}

void file_track::move_modules(std::set<time::range> const &ranges, frame_index_t const offset) {
    std::vector<file_module> movings;
    for (auto const &range : ranges) {
        if (this->_modules.count(range) > 0) {
            auto const &module = this->_modules.at(range);
            movings.emplace_back(module.value.offset(offset));
            this->erase_module_and_notify(range);
        }
    }

    for (auto const &params : movings) {
        this->overwrite_module(params);
    }
}

void file_track::split_and_insert_module_and_offset(file_module const &params) {
    this->split_at(params.range.frame);

    this->_move_modules_after(params.range.frame, params.range.length);
    this->insert_module_and_notify(params);
}

void file_track::_move_modules_after(frame_index_t const frame, frame_index_t const offset) {
    auto const copied_modules = this->_modules;
    for (auto const &pair : copied_modules) {
        if (frame <= pair.first.frame) {
            auto const &moving_module = pair.second.value;
            this->erase_module_and_notify(moving_module.range);
            this->insert_module_and_notify(moving_module.offset(offset));
        }
    }
}

observing::syncable file_track::observe_event(std::function<void(file_track_event const &)> &&handler) {
    return this->_event_fetcher->observe(std::move(handler));
}

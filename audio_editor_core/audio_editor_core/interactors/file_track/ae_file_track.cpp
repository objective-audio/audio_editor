//
//  ae_file_track.cpp
//

#include "ae_file_track.h"

#include <audio_editor_core/ae_file_module_utils.h>
#include <cpp_utils/yas_stl_utils.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<file_track> file_track::make_shared() {
    return std::make_shared<file_track>();
}

file_track::file_track() {
    this->_event_fetcher = observing::fetcher<file_track_event>::make_shared(
        [this] { return file_track_event{.type = file_track_event_type::any, .modules = this->_modules}; });
}

file_track_module_map_t const &file_track::modules() const {
    return this->_modules;
}

void file_track::revert_modules_and_notify(std::vector<file_module> &&modules) {
    this->_modules = to_map<time::range>(modules, [](file_module const &module) { return module.range; });
    this->_event_fetcher->push({.type = file_track_event_type::reverted, .modules = this->_modules});
}

void file_track::insert_module_and_notify(file_module const &module) {
    this->_modules.emplace(module.range, module);
    this->_event_fetcher->push({.type = file_track_event_type::inserted, .module = module, .modules = this->_modules});
}

void file_track::erase_module_and_notify(file_module const &module) {
    if (this->_modules.contains(module.range)) {
        this->_modules.erase(module.range);
        this->_event_fetcher->push(
            {.type = file_track_event_type::erased, .module = module, .modules = this->_modules});
    }
}

void file_track::set_module_name_and_notify(time::range const &range, std::string const &name) {
    if (this->_modules.contains(range)) {
        auto &module = this->_modules.at(range);
        if (module.name != name) {
            module.name = name;
            this->_event_fetcher->push(
                {.type = file_track_event_type::detail_updated, .module = module, .modules = this->_modules});
        }
    }
}

std::optional<file_module> file_track::module_at(frame_index_t const frame) const {
    return file_module_utils::module(this->_modules, frame);
}

std::optional<file_module> file_track::previous_module_at(frame_index_t const frame) const {
    return file_module_utils::previous_module(this->_modules, frame);
}

std::optional<file_module> file_track::next_module_at(frame_index_t const frame) const {
    return file_module_utils::next_module(this->_modules, frame);
}

std::optional<file_module> file_track::splittable_module_at(frame_index_t const frame) const {
    return file_module_utils::splittable_module(this->_modules, frame);
}

std::optional<file_module> file_track::first_module() const {
    return file_module_utils::first_module(this->_modules);
}

std::optional<file_module> file_track::last_module() const {
    return file_module_utils::last_module(this->_modules);
}

std::optional<frame_index_t> file_track::next_jumpable_frame(frame_index_t const frame) const {
    if (auto const &module = this->module_at(frame); module.has_value()) {
        return module->range.next_frame();
    }

    if (auto const &module = this->next_module_at(frame); module.has_value()) {
        return module->range.frame;
    }

    return std::nullopt;
}

std::optional<frame_index_t> file_track::previous_jumpable_frame(frame_index_t const frame) const {
    if (auto const &module = this->module_at(frame); module.has_value()) {
        if (module->range.frame < frame) {
            return module->range.frame;
        }
    }

    if (auto const &module = this->previous_module_at(frame)) {
        if (module->range.next_frame() == frame) {
            return module->range.frame;
        } else {
            return module->range.next_frame();
        }
    }

    return std::nullopt;
}

void file_track::split_at(frame_index_t const frame) {
    if (auto const module_opt = this->splittable_module_at(frame); module_opt.has_value()) {
        auto const &module = module_opt.value();
        this->erase_module_and_notify(module);
        this->insert_module_and_notify(module.tail_dropped(frame).value());
        this->insert_module_and_notify(module.head_dropped(frame).value());
    }
}

void file_track::erase_at(frame_index_t const frame) {
    if (auto const module_opt = this->module_at(frame); module_opt.has_value()) {
        this->erase_module_and_notify(module_opt.value());
    }
}

void file_track::erase_and_offset_at(frame_index_t const frame) {
    if (auto const module_opt = this->module_at(frame); module_opt.has_value()) {
        auto const &module = module_opt.value();
        frame_index_t const offset = -int64_t(module.range.length);

        this->erase_module_and_notify(module);
        this->_move_modules_after(frame, offset);
    }
}

void file_track::drop_head_at(frame_index_t const frame) {
    if (auto const module_opt = this->splittable_module_at(frame); module_opt.has_value()) {
        auto const &module = module_opt.value();
        this->erase_module_and_notify(module);
        this->insert_module_and_notify(module.head_dropped(frame).value());
    }
}

void file_track::drop_tail_at(frame_index_t const frame) {
    if (auto const module_opt = this->splittable_module_at(frame); module_opt.has_value()) {
        auto const &module = module_opt.value();
        this->erase_module_and_notify(module);
        this->insert_module_and_notify(module.tail_dropped(frame).value());
    }
}

void file_track::drop_head_and_offset_at(frame_index_t const frame) {
    if (auto const module_opt = this->splittable_module_at(frame); module_opt.has_value()) {
        auto const &module = module_opt.value();
        frame_index_t const offset = module.range.frame - frame;

        this->erase_module_and_notify(module);
        this->insert_module_and_notify(module.head_dropped(frame).value());
        this->_move_modules_after(frame, offset);
    }
}

void file_track::drop_tail_and_offset_at(frame_index_t const frame) {
    if (auto const module_opt = this->splittable_module_at(frame); module_opt.has_value()) {
        auto const &module = module_opt.value();
        frame_index_t const offset = frame - module.range.next_frame();

        this->erase_module_and_notify(module);
        this->insert_module_and_notify(module.tail_dropped(frame).value());

        this->_move_modules_after(frame, offset);
    }
}

void file_track::overwrite_module(file_module const &module) {
    auto const overlapped_modules = file_module_utils::overlapped_modules(this->_modules, module.range);
    for (auto const &overlapped_module : overlapped_modules) {
        this->erase_module_and_notify(overlapped_module);
        auto const cropped_ranges = overlapped_module.range.cropped(module.range);
        for (auto const &cropped_range : cropped_ranges) {
            frame_index_t const file_frame_offset = cropped_range.frame - overlapped_module.range.frame;
            this->insert_module_and_notify({.name = overlapped_module.name,
                                            .range = cropped_range,
                                            .file_frame = overlapped_module.range.frame + file_frame_offset,
                                            .file_name = overlapped_module.file_name});
        }
    }

    this->insert_module_and_notify(module);
}

void file_track::move_modules(std::set<time::range> const &ranges, frame_index_t const offset) {
    std::vector<file_module> moving_modules;
    for (auto const &range : ranges) {
        if (this->_modules.count(range) > 0) {
            auto const &module = this->_modules.at(range);
            moving_modules.emplace_back(module.offset(offset));
            this->erase_module_and_notify(module);
        }
    }

    for (auto const &moving_module : moving_modules) {
        this->overwrite_module(moving_module);
    }
}

void file_track::split_and_insert_module_and_offset(file_module const &module) {
    this->split_at(module.range.frame);

    this->_move_modules_after(module.range.frame, module.range.length);
    this->insert_module_and_notify(module);
}

void file_track::_move_modules_after(frame_index_t const frame, frame_index_t const offset) {
    auto const copied_modules = this->_modules;
    for (auto const &pair : copied_modules) {
        if (frame <= pair.first.frame) {
            auto const &moving_module = pair.second;
            this->erase_module_and_notify(moving_module);
            this->insert_module_and_notify(moving_module.offset(offset));
        }
    }
}

observing::syncable file_track::observe_event(std::function<void(file_track_event const &)> &&handler) {
    return this->_event_fetcher->observe(std::move(handler));
}

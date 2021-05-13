//
//  ae_file_track.cpp
//

#include "ae_file_track.h"

#include <audio_editor_core/ae_file_module_utils.h>
#include <cpp_utils/yas_stl_utils.h>

using namespace yas;
using namespace yas::ae;

file_track::file_track() {
    this->_event_fetcher = observing::fetcher<file_track_event>::make_shared(
        [this] { return file_track_event{.type = file_track_event_type::any, .modules = this->_modules}; });
}

file_track_module_map_t const &file_track::modules() const {
    return this->_modules;
}

void file_track::replace_modules(std::vector<file_module> &&modules) {
    this->_modules = to_map<proc::time::range>(modules, [](file_module const &module) { return module.range; });
    this->_event_fetcher->push({.type = file_track_event_type::any, .modules = this->_modules});
}

void file_track::insert_module(file_module const &module) {
    this->_modules.emplace(module.range, module);
    this->_event_fetcher->push({.type = file_track_event_type::inserted, .module = module, .modules = this->_modules});
}

void file_track::erase_module(file_module const &module) {
    if (this->_modules.count(module.range) > 0) {
        this->_modules.erase(module.range);
        this->_event_fetcher->push(
            {.type = file_track_event_type::erased, .module = module, .modules = this->_modules});
    }
}

std::optional<file_module> file_track::module(proc::frame_index_t const frame) {
    for (auto const &pair : this->_modules) {
        if (pair.first.is_contain(frame)) {
            return pair.second;
        }
    }
    return std::nullopt;
}

std::optional<file_module> file_track::splittable_module(proc::frame_index_t const frame) const {
    for (auto const &pair : this->_modules) {
        if (file_module_utils::can_split_time_range(pair.second.range, frame)) {
            return pair.second;
        }
    }
    return std::nullopt;
}

void file_track::split(proc::frame_index_t const frame) {
    if (auto const module_opt = this->splittable_module(frame); module_opt.has_value()) {
        auto const &module = module_opt.value();
        this->erase_module(module);
        this->insert_module(module.tail_dropped(frame).value());
        this->insert_module(module.head_dropped(frame).value());
    }
}

void file_track::drop_head(proc::frame_index_t const frame) {
    if (auto const module_opt = this->splittable_module(frame); module_opt.has_value()) {
        auto const &module = module_opt.value();
        this->erase_module(module);
        this->insert_module(module.head_dropped(frame).value());
    }
}

void file_track::drop_tail(proc::frame_index_t const frame) {
    if (auto const module_opt = this->splittable_module(frame); module_opt.has_value()) {
        auto const &module = module_opt.value();
        this->erase_module(module);
        this->insert_module(module.tail_dropped(frame).value());
    }
}

observing::syncable file_track::observe_event(std::function<void(file_track_event const &)> &&handler) {
    return this->_event_fetcher->observe(std::move(handler));
}

std::shared_ptr<file_track> file_track::make_shared() {
    return std::shared_ptr<file_track>(new file_track{});
}

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

module_pool_module_map_t module_pool::modules_at(std::set<track_index_t> const &tracks,
                                                 frame_index_t const frame) const {
    return module_pool_utils::modules(this->_modules, tracks, frame);
}

bool module_pool::has_modules_at(std::set<track_index_t> const &tracks, frame_index_t const frame) const {
    return module_pool_utils::has_modules(this->_modules, tracks, frame);
}

module_pool_module_map_t module_pool::splittable_modules_at(std::set<track_index_t> const &tracks,
                                                            frame_index_t const frame) const {
    return module_pool_utils::splittable_modules(this->_modules, tracks, frame);
}

bool module_pool::has_splittable_modules_at(std::set<track_index_t> const &tracks, frame_index_t const frame) const {
    return module_pool_utils::has_splittable_modules(this->_modules, tracks, frame);
}

bool module_pool::has_splittable_modules(std::set<module_index> const &module_indices,
                                         frame_index_t const frame) const {
    return module_pool_utils::has_splittable_modules(module_indices, {}, frame);
}

std::optional<frame_index_t> module_pool::first_frame() const {
    return module_pool_utils::first_frame(this->_modules);
}

std::optional<frame_index_t> module_pool::last_next_frame() const {
    return module_pool_utils::last_next_frame(this->_modules);
}

std::optional<frame_index_t> module_pool::next_jumpable_frame(frame_index_t const frame) const {
    return module_pool_utils::next_jumpable_frame(this->_modules, frame);
}

std::optional<frame_index_t> module_pool::previous_jumpable_frame(frame_index_t const frame) const {
    return module_pool_utils::previous_jumpable_frame(this->_modules, frame);
}

std::optional<track_index_t> module_pool::first_track() const {
    std::optional<track_index_t> result = std::nullopt;

    for (auto const &pair : this->_modules) {
        auto const &track = pair.first.track;
        if (result.has_value()) {
            if (track < result.value()) {
                result = track;
            }
        } else {
            result = track;
        }
    }

    return result;
}

std::optional<track_index_t> module_pool::last_track() const {
    std::optional<track_index_t> result = std::nullopt;

    for (auto const &pair : this->_modules) {
        auto const &track = pair.first.track;
        if (result.has_value()) {
            if (result.value() < track) {
                result = track;
            }
        } else {
            result = track;
        }
    }

    return result;
}

void module_pool::split_at(std::set<track_index_t> const &tracks, frame_index_t const frame) {
    auto const modules = this->splittable_modules_at(tracks, frame);
    this->_split_modules(modules, frame);
}

void module_pool::split(selected_module_set const &module_indices, frame_index_t const frame) {
    auto const modules = module_pool_utils::splittable_modules(this->_modules, module_indices, frame);
    this->_split_modules(modules, frame);
}

void module_pool::erase_at(std::set<track_index_t> const &tracks, frame_index_t const frame) {
    if (auto const modules = this->modules_at(tracks, frame); !modules.empty()) {
        for (auto const &pair : modules) {
            this->erase_module_and_notify(pair.second.index());
        }
    }
}

void module_pool::drop_head_at(std::set<track_index_t> const &tracks, frame_index_t const frame) {
    auto const modules = this->splittable_modules_at(tracks, frame);
    this->_drop_head_modules(modules, frame);
}

void module_pool::drop_head(selected_module_set const &module_indices, frame_index_t const frame) {
    auto const modules = module_pool_utils::splittable_modules(this->_modules, module_indices, frame);
    this->_drop_head_modules(modules, frame);
}

void module_pool::drop_tail_at(std::set<track_index_t> const &tracks, frame_index_t const frame) {
    auto const modules = this->splittable_modules_at(tracks, frame);
    this->_drop_tail_modules(modules, frame);
}

void module_pool::drop_tail(selected_module_set const &module_indices, frame_index_t const frame) {
    auto const modules = module_pool_utils::splittable_modules(this->_modules, module_indices, frame);
    this->_drop_tail_modules(modules, frame);
}

void module_pool::overwrite_module(module const &module) {
    auto const overlapped_modules = module_pool_utils::overlapped_modules(this->_modules, module.track, module.range);
    for (auto const &overlapped_module : overlapped_modules) {
        this->erase_module_and_notify(overlapped_module.index());
        auto const cropped_ranges = overlapped_module.value.range.cropped(module.range);
        for (auto const &cropped_range : cropped_ranges) {
            frame_index_t const file_frame_offset = cropped_range.frame - overlapped_module.value.range.frame;
            this->insert_module_and_notify({overlapped_module.value.name, cropped_range, overlapped_module.value.track,
                                            overlapped_module.value.file_frame + file_frame_offset,
                                            overlapped_module.value.file_name});
        }
    }

    this->insert_module_and_notify(module);
}

observing::syncable module_pool::observe_event(std::function<void(module_pool_event const &)> &&handler) {
    return this->_event_fetcher->observe(std::move(handler));
}

void module_pool::_split_modules(module_pool_module_map_t const &modules, frame_index_t const frame) {
    for (auto const &pair : modules) {
        auto const &module_obj = pair.second;
        this->erase_module_and_notify(module_obj.index());
        this->insert_module_and_notify(module_obj.value.tail_dropped(frame).value());
        this->insert_module_and_notify(module_obj.value.head_dropped(frame).value());
    }
}

void module_pool::_drop_head_modules(module_pool_module_map_t const &modules, frame_index_t const frame) {
    for (auto const &pair : modules) {
        auto const &module_obj = pair.second;
        this->erase_module_and_notify(module_obj.index());
        this->insert_module_and_notify(module_obj.value.head_dropped(frame).value());
    }
}

void module_pool::_drop_tail_modules(module_pool_module_map_t const &modules, frame_index_t const frame) {
    for (auto const &pair : modules) {
        auto const &module_obj = pair.second;
        this->erase_module_and_notify(module_obj.index());
        this->insert_module_and_notify(module_obj.value.tail_dropped(frame).value());
    }
}

//
//  ae_file_track_utils.cpp
//

#include "ae_file_track_utils.h"

#include <audio_editor_core/ae_file_module_utils.h>

using namespace yas;
using namespace yas::ae;

std::optional<time::range> file_track_utils::total_range(file_track_module_map_t const &modules) {
    std::optional<time::range> result{std::nullopt};

    for (auto const &pair : modules) {
        if (result.has_value()) {
            result = result->merged(pair.first.range);
        } else {
            result = pair.first.range;
        }
    }

    return result;
}

std::optional<file_module_object> file_track_utils::module(file_track_module_map_t const &modules,
                                                           frame_index_t const frame) {
    for (auto const &pair : modules) {
        if (pair.first.range.is_contain(frame)) {
            return pair.second;
        }
    }
    return std::nullopt;
}

std::optional<file_module_object> file_track_utils::first_module(file_track_module_map_t const &modules) {
    auto const iterator = modules.cbegin();
    if (iterator != modules.cend()) {
        return iterator->second;
    }
    return std::nullopt;
}

std::optional<file_module_object> file_track_utils::last_module(file_track_module_map_t const &modules) {
    auto const iterator = modules.crbegin();
    if (iterator != modules.crend()) {
        return iterator->second;
    }
    return std::nullopt;
}

std::optional<file_module_object> file_track_utils::previous_module(file_track_module_map_t const &modules,
                                                                    frame_index_t const frame) {
    auto it = modules.rbegin();

    while (it != modules.rend()) {
        auto const &pair = *it;
        if (pair.first.range.next_frame() <= frame) {
            return pair.second;
        }
        ++it;
    }

    return std::nullopt;
}

std::optional<file_module_object> file_track_utils::next_module(file_track_module_map_t const &modules,
                                                                frame_index_t const frame) {
    for (auto const &pair : modules) {
        if (frame < pair.first.range.frame) {
            return pair.second;
        }
    }

    return std::nullopt;
}

std::optional<file_module_object> file_track_utils::splittable_module(file_track_module_map_t const &modules,
                                                                      frame_index_t const frame) {
    for (auto const &pair : modules) {
        if (file_module_utils::can_split_time_range(pair.second.value.range, frame)) {
            return pair.second;
        }
    }
    return std::nullopt;
}

std::vector<file_module_object> file_track_utils::overlapped_modules(file_track_module_map_t const &modules,
                                                                     time::range const &range) {
    auto const next_frame = range.next_frame();

    std::vector<file_module_object> result;
    for (auto const &pair : modules) {
        auto const &module_range = pair.first.range;
        if (next_frame <= module_range.frame) {
            break;
        }
        if (module_range.is_overlap(range)) {
            result.emplace_back(pair.second);
        }
    }
    return result;
}

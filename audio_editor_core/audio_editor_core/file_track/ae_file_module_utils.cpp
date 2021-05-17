//
//  ae_file_module_utils.cpp
//

#include "ae_file_module_utils.h"

using namespace yas;
using namespace yas::ae;

std::optional<file_module> file_module_utils::module(file_track_module_map_t const &modules,
                                                     proc::frame_index_t const frame) {
    for (auto const &pair : modules) {
        if (pair.first.is_contain(frame)) {
            return pair.second;
        }
    }
    return std::nullopt;
}

std::optional<file_module> file_module_utils::previous_module(file_track_module_map_t const &modules,
                                                              proc::frame_index_t const frame) {
    auto it = modules.rbegin();

    while (it != modules.rend()) {
        auto const &pair = *it;
        if (pair.first.next_frame() <= frame) {
            return pair.second;
        }
        ++it;
    }

    return std::nullopt;
}

std::optional<file_module> file_module_utils::next_module(file_track_module_map_t const &modules,
                                                          proc::frame_index_t const frame) {
    for (auto const &pair : modules) {
        if (frame < pair.first.frame) {
            return pair.second;
        }
    }

    return std::nullopt;
}

bool file_module_utils::can_split_time_range(proc::time::range const &range, proc::frame_index_t const frame) {
    return range.is_contain(frame) && range.frame != frame;
}

std::optional<file_module> file_module_utils::splittable_module(file_track_module_map_t const &modules,
                                                                proc::frame_index_t const frame) {
    for (auto const &pair : modules) {
        if (can_split_time_range(pair.second.range, frame)) {
            return pair.second;
        }
    }
    return std::nullopt;
}

std::vector<file_module> file_module_utils::overlapped_modules(file_track_module_map_t const &modules,
                                                               proc::time::range const &range) {
    auto const next_frame = range.next_frame();

    std::vector<file_module> result;
    for (auto const &pair : modules) {
        auto const &module_range = pair.first;
        if (next_frame <= module_range.frame) {
            break;
        }
        if (module_range.is_overlap(range)) {
            result.emplace_back(pair.second);
        }
    }
    return result;
}
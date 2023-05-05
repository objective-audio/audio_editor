//
//  ae_module_pool_utils.cpp
//

#include "ae_module_pool_utils.h"

#include <audio_editor_core/ae_module_utils.h>

using namespace yas;
using namespace yas::ae;

std::optional<time::range> module_pool_utils::total_range(module_pool_module_map_t const &modules) {
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

std::optional<module_object> module_pool_utils::module(module_pool_module_map_t const &modules,
                                                       frame_index_t const frame) {
    for (auto const &pair : modules) {
        if (pair.first.range.is_contain(frame)) {
            return pair.second;
        }
    }
    return std::nullopt;
}

std::optional<module_object> module_pool_utils::first_module(module_pool_module_map_t const &modules) {
    auto const iterator = modules.cbegin();
    if (iterator != modules.cend()) {
        return iterator->second;
    }
    return std::nullopt;
}

std::optional<module_object> module_pool_utils::last_module(module_pool_module_map_t const &modules) {
    auto const iterator = modules.crbegin();
    if (iterator != modules.crend()) {
        return iterator->second;
    }
    return std::nullopt;
}

std::optional<module_object> module_pool_utils::previous_module(module_pool_module_map_t const &modules,
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

std::optional<module_object> module_pool_utils::next_module(module_pool_module_map_t const &modules,
                                                            frame_index_t const frame) {
    for (auto const &pair : modules) {
        if (frame < pair.first.range.frame) {
            return pair.second;
        }
    }

    return std::nullopt;
}

std::optional<module_object> module_pool_utils::splittable_module(module_pool_module_map_t const &modules,
                                                                  frame_index_t const frame) {
    for (auto const &pair : modules) {
        if (module_utils::can_split_time_range(pair.second.value.range, frame)) {
            return pair.second;
        }
    }
    return std::nullopt;
}

std::vector<module_object> module_pool_utils::overlapped_modules(module_pool_module_map_t const &modules,
                                                                 time::range const &range) {
    auto const next_frame = range.next_frame();

    std::vector<module_object> result;
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

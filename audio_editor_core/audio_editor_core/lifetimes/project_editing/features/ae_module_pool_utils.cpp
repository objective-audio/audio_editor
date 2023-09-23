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

module_pool_module_map_t module_pool_utils::modules(module_pool_module_map_t const &modules,
                                                    std::set<track_index_t> const &tracks, frame_index_t const frame) {
    module_pool_module_map_t result;

    bool const is_tracks_empty = tracks.empty();

    for (auto const &pair : modules) {
        // 格納されている順番的にmoduleのframeの方が大きくなったら被らないはずなので打ち切る
        if (frame < pair.first.range.frame) {
            break;
        }

        if ((is_tracks_empty || tracks.contains(pair.first.track)) && pair.first.range.is_contain(frame)) {
            result.emplace(pair);
        }
    }

    return result;
}

bool module_pool_utils::has_modules(module_pool_module_map_t const &modules, std::set<track_index_t> const &tracks,
                                    frame_index_t const frame) {
    if (tracks.empty()) {
        return false;
    }

    for (auto const &pair : modules) {
        // 格納されている順番的にmoduleのframeの方が大きくなったら被らないはずなので打ち切る
        if (frame < pair.first.range.frame) {
            break;
        }

        if (tracks.contains(pair.first.track) && pair.first.range.is_contain(frame)) {
            return true;
        }
    }

    return false;
}

std::optional<frame_index_t> module_pool_utils::first_frame(module_pool_module_map_t const &modules) {
    auto const iterator = modules.cbegin();
    if (iterator != modules.cend()) {
        return iterator->first.range.frame;
    }
    return std::nullopt;
}

std::optional<frame_index_t> module_pool_utils::last_next_frame(module_pool_module_map_t const &modules) {
    std::optional<frame_index_t> result;

    for (auto const &pair : modules) {
        if (result.has_value()) {
            result = std::max(result.value(), pair.first.range.next_frame());
        } else {
            result = pair.first.range.next_frame();
        }
    }

    return result;
}

std::optional<frame_index_t> module_pool_utils::previous_jumpable_frame(module_pool_module_map_t const &modules,
                                                                        frame_index_t const frame) {
    std::optional<frame_index_t> result;

    for (auto const &pair : modules) {
        auto const &module_frame = pair.first.range.frame;

        if (frame <= module_frame) {
            break;
        }

        std::optional<frame_index_t> current_frame;

        auto const module_next_frame = pair.first.range.next_frame();

        if (module_next_frame < frame) {
            current_frame = module_next_frame;
        } else if (module_frame < frame) {
            current_frame = module_frame;
        } else {
            continue;
        }

        if (result.has_value()) {
            result = std::max(result.value(), current_frame.value());
        } else {
            result = current_frame;
        }
    }

    return result;
}

std::optional<frame_index_t> module_pool_utils::next_jumpable_frame(module_pool_module_map_t const &modules,
                                                                    frame_index_t const frame) {
    std::optional<frame_index_t> result;

    for (auto const &pair : modules) {
        auto const &module_frame = pair.first.range.frame;
        auto const module_next_frame = pair.first.range.next_frame();

        std::optional<frame_index_t> current_frame;

        if (frame < module_frame) {
            current_frame = module_frame;
        } else if (frame < module_next_frame) {
            current_frame = module_next_frame;
        }

        if (result.has_value()) {
            result = std::min(result.value(), current_frame.value());
        } else {
            result = current_frame;
        }

        if (frame <= module_frame) {
            break;
        }
    }

    return result;
}

module_pool_module_map_t module_pool_utils::splittable_modules(module_pool_module_map_t const &modules,
                                                               std::set<track_index_t> const &tracks,
                                                               frame_index_t const frame) {
    module_pool_module_map_t result;

    bool const is_tracks_empty = tracks.empty();

    for (auto const &pair : modules) {
        if (frame < pair.first.range.frame) {
            break;
        }

        if ((is_tracks_empty || tracks.contains(pair.first.track)) &&
            module_utils::can_split_time_range(pair.first.range, frame)) {
            result.emplace(pair);
        }
    }

    return result;
}

std::vector<module_object> module_pool_utils::overlapped_modules(module_pool_module_map_t const &modules,
                                                                 track_index_t const track, time::range const &range) {
    auto const next_frame = range.next_frame();

    std::vector<module_object> result;
    for (auto const &pair : modules) {
        auto const &module_range = pair.first.range;
        if (next_frame <= module_range.frame) {
            break;
        }
        if (pair.first.track == track && module_range.is_overlap(range)) {
            result.emplace_back(pair.second);
        }
    }
    return result;
}

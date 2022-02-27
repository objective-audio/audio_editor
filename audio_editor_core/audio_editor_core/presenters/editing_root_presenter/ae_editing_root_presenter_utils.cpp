//
//  ae_editing_root_presenter_utils.cpp
//

#include "ae_editing_root_presenter_utils.h"

#include <audio_editor_core/ae_file_track.h>
#include <audio_editor_core/ae_marker_pool.h>
#include <playing/yas_playing_math.h>

#include <iomanip>
#include <sstream>

using namespace yas;
using namespace yas::ae;

std::string const &editing_root_presenter_utils::empty_text() {
    static std::string const text = "";
    return text;
}

std::string editing_root_presenter_utils::label_text(project_state const &state) {
    switch (state) {
        case ae::project_state::launching:
            return "launching";
        case ae::project_state::loading:
            return "loading";
        case ae::project_state::editing:
            return "editing";
        case ae::project_state::failure:
            return "failure";
        case ae::project_state::closing:
            return "closing";
    }
}

std::string editing_root_presenter_utils::label_text(std::optional<file_info> const &file_info) {
    if (file_info.has_value()) {
        return "sample rate : " + std::to_string(file_info.value().sample_rate) +
               " / channel count : " + std::to_string(file_info.value().channel_count) +
               " / length : " + std::to_string(file_info.value().length);
    } else {
        return "-";
    }
}

std::string editing_root_presenter_utils::play_button_text(bool const is_playing) {
    return is_playing ? "played" : "paused";
}

std::string editing_root_presenter_utils::file_track_text(file_track_module_map_t const &modules) {
    std::vector<std::string> module_texts;

    for (auto const &pair : modules) {
        file_module const &module = pair.second;
        auto text = "range:" + to_string(module.range) + "/frame:" + std::to_string(module.file_frame);
        module_texts.emplace_back(std::move(text));
    }

    if (module_texts.empty()) {
        return "--";
    } else {
        return joined(module_texts, "\n");
    }
}

std::string editing_root_presenter_utils::marker_pool_text(marker_map_t const &markers) {
    std::vector<std::string> texts;

    std::size_t idx = 0;

    for (auto const &pair : markers) {
        marker const &marker = pair.second;
        texts.emplace_back("marker index:" + std::to_string(idx) + "/frame:" + std::to_string(marker.frame));
        ++idx;
    }

    if (texts.empty()) {
        return "--";
    } else {
        return joined(texts, "\n");
    }
}

std::string editing_root_presenter_utils::timing_fraction_text(timing_fraction_kind const kind) {
    switch (kind) {
        case timing_fraction_kind::sample:
            return "sample";
        case timing_fraction_kind::milisecond:
            return "ms";
        case timing_fraction_kind::frame30:
            return "30fps";
    }
}

std::string editing_root_presenter_utils::timing_text(timing_fraction_kind const kind) {
    return "timing\n" + timing_fraction_text(kind);
}

observing::fetcher_ptr<file_track_event> editing_root_presenter_utils::make_file_track_fetcher(
    std::shared_ptr<file_track> const &file_track) {
    return observing::fetcher<file_track_event>::make_shared([weak_file_track = to_weak(file_track)] {
        if (auto const file_track = weak_file_track.lock()) {
            return file_track_event({.type = file_track_event_type::any, .modules = file_track->modules()});
        }
        return file_track_event({.type = file_track_event_type::any, .modules = ae::empty_file_track_modules});
    });
}

observing::fetcher_ptr<marker_pool_event> editing_root_presenter_utils::make_marker_pool_fetcher(
    std::shared_ptr<marker_pool> const &marker_pool) {
    return observing::fetcher<marker_pool_event>::make_shared([weak_marker_pool = to_weak(marker_pool)] {
        if (auto const &marker_pool = weak_marker_pool.lock()) {
            return marker_pool_event({.type = marker_pool_event_type::any, .markers = marker_pool->markers()});
        }
        return marker_pool_event({.type = marker_pool_event_type::any, .markers = ae::empty_markers});
    });
}

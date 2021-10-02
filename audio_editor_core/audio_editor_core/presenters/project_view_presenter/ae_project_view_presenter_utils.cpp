//
//  ae_project_view_presenter_utils.cpp
//

#include "ae_project_view_presenter_utils.h"

#include <audio_editor_core/ae_file_track_types.h>
#include <playing/yas_playing_math.h>

#include <iomanip>
#include <sstream>

using namespace yas;
using namespace yas::ae;

std::string const &project_view_presenter_utils::empty_text() {
    static std::string const text = "";
    return text;
}

std::string project_view_presenter_utils::label_text(project_state const &state) {
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

std::string project_view_presenter_utils::label_text(std::optional<file_info> const &file_info) {
    if (file_info.has_value()) {
        return "sample rate : " + std::to_string(file_info.value().sample_rate) +
               " / channel count : " + std::to_string(file_info.value().channel_count) +
               " / length : " + std::to_string(file_info.value().length);
    } else {
        return "-";
    }
}

std::string project_view_presenter_utils::player_text(frame_index_t const frame, uint32_t const sample_rate) {
    std::string const time = time_text(frame, sample_rate);
    return time + " [" + std::to_string(frame) + "]";
}

std::string project_view_presenter_utils::play_button_text(bool const is_playing) {
    return is_playing ? "played" : "paused";
}

std::string project_view_presenter_utils::file_track_text(file_track_module_map_t const &modules) {
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

std::string project_view_presenter_utils::marker_pool_text(marker_map_t const &markers) {
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

int project_view_presenter_utils::after_point_digits(uint32_t const sample_rate) {
    if (sample_rate == 0) {
        throw std::invalid_argument("sample_rate is zero.");
    }

    if (sample_rate == 1) {
        return 0;
    }

    uint32_t value = 10;
    uint32_t digits = 1;
    uint32_t prev_mod = sample_rate;

    while (true) {
        if ((sample_rate / value) == 0) {
            if (prev_mod == 0) {
                digits -= 1;
            }
            break;
        }

        prev_mod = sample_rate % value;

        digits += 1;
        value *= 10;
    }

    return static_cast<int>(digits);
}

std::string project_view_presenter_utils::time_text(int64_t const frame, uint32_t const sample_rate) {
    bool const is_minus = frame < 0;
    int64_t const abs_frame = std::abs(frame);

    auto const digits = after_point_digits(sample_rate);

    int64_t const after_point = playing::math::mod_int(abs_frame, sample_rate);
    int64_t const sec = playing::math::floor_int(abs_frame, sample_rate) / sample_rate;
    int64_t const mod_sec = playing::math::mod_int(sec, 60);
    int64_t const min = sec / 60;
    int64_t const mod_min = playing::math::mod_int(min, 60);
    int64_t const hour = min / 60;

    std::stringstream stream;

    if (is_minus) {
        stream << "-";
    }

    stream << std::to_string(hour) << ":";
    stream << std::setw(2) << std::setfill('0') << mod_min;
    stream << ":";
    stream << std::setw(2) << std::setfill('0') << mod_sec;

    if (digits > 0) {
        stream << ".";
        stream << std::setw(digits) << std::setfill('0') << after_point;
    }

    return stream.str();
}

observing::fetcher_ptr<file_track_event> project_view_presenter_utils::make_file_track_fetcher(
    std::shared_ptr<project> const &project) {
    return observing::fetcher<file_track_event>::make_shared([weak_project = to_weak(project)] {
        if (auto const project = weak_project.lock()) {
            if (auto const &editor = project->editor()) {
                return file_track_event(
                    {.type = file_track_event_type::any, .modules = editor->file_track()->modules()});
            }
        }
        return file_track_event({.type = file_track_event_type::any, .modules = ae::empty_file_track_modules});
    });
}

observing::fetcher_ptr<marker_pool_event> project_view_presenter_utils::make_marker_pool_fetcher(
    std::shared_ptr<project> const &project) {
    return observing::fetcher<marker_pool_event>::make_shared([weak_project = to_weak(project)] {
        if (auto const project = weak_project.lock()) {
            if (auto const &editor = project->editor()) {
                return marker_pool_event(
                    {.type = marker_pool_event_type::any, .markers = editor->marker_pool()->markers()});
            }
        }
        return marker_pool_event({.type = marker_pool_event_type::any, .markers = ae::empty_markers});
    });
}

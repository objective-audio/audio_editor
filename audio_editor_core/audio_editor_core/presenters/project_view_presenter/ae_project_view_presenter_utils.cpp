//
//  ae_project_view_presenter_utils.cpp
//

#include "ae_project_view_presenter_utils.h"

#include <audio_editor_core/ae_file_track_types.h>

using namespace yas;
using namespace yas::ae;

std::string project_view_presenter_utils::to_label_text(project_state const &state) {
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

std::string project_view_presenter_utils::to_label_text(std::optional<file_info> const &file_info) {
    if (file_info.has_value()) {
        return "sample rate : " + std::to_string(file_info.value().sample_rate) +
               " / channel count : " + std::to_string(file_info.value().channel_count) +
               " / length : " + std::to_string(file_info.value().length);
    } else {
        return "-";
    }
}

std::string project_view_presenter_utils::to_player_text(frame_index_t const current_frame) {
    return "current : " + std::to_string(current_frame);
}

std::string project_view_presenter_utils::to_play_button_text(bool const is_playing) {
    return is_playing ? "played" : "paused";
}

std::string project_view_presenter_utils::to_file_track_text(file_track_module_map_t const &modules) {
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

observing::fetcher_ptr<file_track_event> project_view_presenter_utils::make_fetcher(
    std::shared_ptr<project> const &project) {
    return observing::fetcher<file_track_event>::make_shared([weak_project = to_weak(project)] {
        if (auto const &project = weak_project.lock()) {
            if (auto const &editor = project->editor()) {
                return file_track_event(
                    {.type = file_track_event_type::any, .modules = editor->file_track()->modules()});
            }
        }
        static file_track_module_map_t const _empty_modules;
        return file_track_event({.type = file_track_event_type::any, .modules = _empty_modules});
    });
}

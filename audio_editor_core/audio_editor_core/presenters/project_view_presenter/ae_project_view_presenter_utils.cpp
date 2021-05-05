//
//  ae_project_view_presenter_utils.cpp
//

#include "ae_project_view_presenter_utils.h"

using namespace yas;
using namespace yas::ae;

std::string project_view_presenter_utils::to_label_string(project_state const &state) {
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

std::string project_view_presenter_utils::to_label_string(std::optional<file_info> const &file_info) {
    if (file_info.has_value()) {
        return "sample rate : " + std::to_string(file_info.value().sample_rate) +
               " / length : " + std::to_string(file_info.value().length);
    } else {
        return "-";
    }
}

std::string project_view_presenter_utils::to_player_string(frame_index_t const current_frame) {
    return "current : " + std::to_string(current_frame);
}

std::string project_view_presenter_utils::to_play_button_string(bool const is_playing) {
    return is_playing ? "played" : "paused";
}

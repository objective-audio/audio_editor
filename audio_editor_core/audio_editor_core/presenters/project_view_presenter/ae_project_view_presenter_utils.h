//
//  ae_project_view_presenter_utils.h
//

#pragma once

#include <audio_editor_core/ae_file_loader_types.h>
#include <audio_editor_core/ae_player_types.h>
#include <audio_editor_core/ae_project.h>

#include <optional>
#include <string>

namespace yas::ae::project_view_presenter_utils {
std::string label_text(project_state const &state);
std::string label_text(std::optional<file_info> const &file_info);
std::string player_text(frame_index_t const current_frame);
std::string play_button_text(bool const);
std::string file_track_text(file_track_module_map_t const &);

observing::fetcher_ptr<file_track_event> make_fetcher(std::shared_ptr<project> const &);
}  // namespace yas::ae::project_view_presenter_utils

//
//  ae_editing_root_presenter_utils.h
//

#pragma once

#include <audio_editor_core/ae_common_types.h>
#include <audio_editor_core/ae_editing_root_presenter_dependency.h>
#include <audio_editor_core/ae_file_loader_types.h>
#include <audio_editor_core/ae_marker_pool_types.h>
#include <audio_editor_core/ae_nudging_types.h>
#include <audio_editor_core/ae_project.h>

#include <optional>
#include <string>

namespace yas::ae::editing_root_presenter_utils {
std::string const &empty_text();
std::string label_text(project_state const &state);
std::string label_text(std::optional<file_info> const &file_info);
std::string play_button_text(bool const);
std::string file_track_text(file_track_module_map_t const &);
std::string marker_pool_text(marker_map_t const &);
std::string nudge_kind_text(nudging_kind const);
std::string nudge_text(nudging_kind const);
std::string timing_fraction_text(timing_fraction_kind const);
std::string timing_text(timing_fraction_kind const);

observing::fetcher_ptr<file_track_event> make_file_track_fetcher(
    std::shared_ptr<project_editor_for_editing_root_presenter> const &);
observing::fetcher_ptr<marker_pool_event> make_marker_pool_fetcher(
    std::shared_ptr<project_editor_for_editing_root_presenter> const &);
}  // namespace yas::ae::editing_root_presenter_utils

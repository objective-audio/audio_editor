//
//  ae_editing_root_presenter_utils.h
//

#pragma once

#include <audio_editor_core/ae_editing_root_presenter_dependency.h>
#include <audio_editor_core/ae_file_info.h>
#include <audio_editor_core/ae_file_track_types.h>
#include <audio_editor_core/ae_marker_pool_types.h>
#include <audio_editor_core/ae_nudging_types.h>
#include <audio_editor_core/ae_project_types.h>
#include <audio_editor_core/ae_timing_types.h>
#include <observing/yas_observing_umbrella.h>

namespace yas::ae {
class file_track;
class marker_pool;
}  // namespace yas::ae

namespace yas::ae::editing_root_presenter_utils {
std::string const &empty_text();
std::string label_text(project_state const &state);
std::string label_text(std::optional<file_info> const &file_info);
std::string play_button_text(bool const);
std::string file_track_text(file_track_module_map_t const &);
std::string marker_pool_text(marker_map_t const &);
std::string timing_fraction_text(timing_fraction_kind const);
std::string timing_text(timing_fraction_kind const);

observing::fetcher_ptr<file_track_event> make_file_track_fetcher(std::shared_ptr<file_track> const &);
observing::fetcher_ptr<marker_pool_event> make_marker_pool_fetcher(std::shared_ptr<marker_pool> const &);
}  // namespace yas::ae::editing_root_presenter_utils

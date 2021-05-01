//
//  ae_project_view_presenter_utils.h
//

#pragma once

#include <audio_editor_core/ae_file_loader_types.h>
#include <audio_editor_core/ae_project_types.h>

#include <optional>
#include <string>

namespace yas::ae::project_view_presenter_utils {
std::string to_label_string(project_state const &state);
std::string to_label_string(std::optional<file_info> const &file_info);
}  // namespace yas::ae::project_view_presenter_utils

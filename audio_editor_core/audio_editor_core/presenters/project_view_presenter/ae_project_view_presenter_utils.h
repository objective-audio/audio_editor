//
//  ae_project_view_presenter_utils.h
//

#pragma once

#include <audio_editor_core/ae_project_types.h>

#include <string>

namespace yas::ae::project_view_presenter_utils {
std::string to_label_string(project_state const &state);
}

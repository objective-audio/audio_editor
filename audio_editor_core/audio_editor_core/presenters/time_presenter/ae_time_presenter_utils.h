//
//  ae_time_presenter_utils.h
//

#pragma once

#include <audio_editor_core/ae_time_presenter_dependency.h>

#include <string>

namespace yas::ae::time_presenter_utils {
std::string to_sign_string(number_components const &);
std::string to_string(number_components_unit const &);

std::string time_text(number_components const &);
}  // namespace yas::ae::time_presenter_utils

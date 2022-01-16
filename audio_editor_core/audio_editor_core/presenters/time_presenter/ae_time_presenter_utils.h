//
//  ae_time_presenter_utils.h
//

#pragma once

#include <audio_editor_core/ae_time_presenter_dependency.h>

#include <string>

namespace yas::ae::time_presenter_utils {
std::string to_sign_string(timing_components const &);
std::string to_string(number_components::unit const &);

std::string time_text(frame_index_t const frame, std::shared_ptr<timing_for_time_presenter> const &);
}  // namespace yas::ae::time_presenter_utils

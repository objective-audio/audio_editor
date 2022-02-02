//
//  ae_time_presenter_utils.h
//

#pragma once

#include <audio_editor_core/ae_nudging_types.h>
#include <audio_editor_core/ae_time_presenter_dependency.h>
#include <cpp_utils/yas_index_range.h>

#include <string>

namespace yas::ae::time_presenter_utils {
std::string to_sign_string(number_components const &);
std::string to_string(number_components_unit const &);

std::string time_text(number_components const &);

index_range to_time_text_range(number_components const &, std::size_t const unit_idx);
}  // namespace yas::ae::time_presenter_utils

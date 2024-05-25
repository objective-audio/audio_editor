//
//  time_numbers_presenter_utils.h
//

#pragma once

#include <ae-core/global/value_types/number_components.h>
#include <cpp-utils/index_range.h>

#include <string>

namespace yas::ae::time_numbers_presenter_utils {
std::string to_sign_string(number_components const &);
std::string to_string(number_components_unit const &);

std::string time_text(number_components const &);

index_range to_time_text_range(number_components const &, std::size_t const unit_idx);
}  // namespace yas::ae::time_numbers_presenter_utils

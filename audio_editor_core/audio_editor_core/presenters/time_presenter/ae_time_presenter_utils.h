//
//  ae_time_presenter_utils.h
//

#pragma once

#include <audio_editor_core/ae_time_presenter_dependency.h>

#include <cstdint>
#include <string>

namespace yas::ae::time_presenter_utils {
std::string time_text(int64_t const frame, std::shared_ptr<timing_for_time_presenter> const &);
}  // namespace yas::ae::time_presenter_utils

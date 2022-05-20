//
//  ae_project_sub_level.h
//

#pragma once

#include <audio_editor_core/ae_time_editor_level.h>

#include <variant>

namespace yas::ae {
using project_sub_level = std::variant<std::shared_ptr<time_editor_level>>;
}  // namespace yas::ae

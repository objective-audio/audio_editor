//
//  ae_pasteboard_types.h
//

#pragma once

#include <audio_editor_core/ae_pasting_file_module.hpp>
#include <variant>

namespace yas::ae {
enum class pasting_kind {
    file_module,
};

enum class pasteboard_event {
    fetched,
    file_module,
    cleared,
};

using pasting_value = std::variant<std::vector<pasting_file_module_object>>;
}  // namespace yas::ae

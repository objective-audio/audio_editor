//
//  ae_sheet_content.h
//

#pragma once

#include <audio_editor_core/ae_common_types.h>

#include <string>
#include <variant>

namespace yas::ae {
enum class sheet_kind { module_name };

using sheet_value = std::variant<time::range>;

struct sheet_content final {
    sheet_kind kind;
    sheet_value value;

    time::range const &range_value() const;
};
}  // namespace yas::ae

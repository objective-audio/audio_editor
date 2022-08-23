//
//  ae_sheet_content.h
//

#pragma once

#include <audio_editor_core/ae_common_types.h>
#include <cpp_utils/yas_identifier.h>

#include <audio_editor_core/ae_module_name_editor.hpp>
#include <string>
#include <variant>

namespace yas::ae {
enum class sheet_kind { module_name, marker_name };

using sheet_value = std::variant<std::shared_ptr<module_name_editor>, int64_t>;

struct sheet_content final {
    sheet_kind kind;
    sheet_value value;

    [[nodiscard]] std::shared_ptr<module_name_editor> const &module_name_editor() const;
    [[nodiscard]] int64_t const &integer_value() const;
};
}  // namespace yas::ae

//
//  ae_sheet_content.h
//

#pragma once

#include <string>

namespace yas::ae {
enum class sheet_kind { module_name };

struct sheet_content final {
    sheet_kind kind;
    std::string value = "";
};
}  // namespace yas::ae

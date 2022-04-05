//
//  ae_sheet_presenter_types.h
//

#pragma once

#include <string>

namespace yas::ae {
enum class sheet_kind { module_name };

struct sheet_event final {
    sheet_kind kind;
    std::string value = "";
};
}  // namespace yas::ae

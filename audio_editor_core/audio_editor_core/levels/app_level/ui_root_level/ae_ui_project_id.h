//
//  ae_ui_project_id.h
//

#pragma once

#include <string>

namespace yas::ae {
struct ui_project_id final {
    std::string const identifier;
    std::uintptr_t const view_id;

    bool operator==(ui_project_id const &);
    bool operator!=(ui_project_id const &);
};
}  // namespace yas::ae

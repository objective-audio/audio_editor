//
//  ae_ui_project_id.h
//

#pragma once

#include <audio_editor_core/ae_project_id.h>

#include <string>

namespace yas::ae {
struct ui_project_id final {
    project_id const project_id;
    std::uintptr_t const view_id;

    bool operator==(ui_project_id const &);
    bool operator!=(ui_project_id const &);
};
}  // namespace yas::ae

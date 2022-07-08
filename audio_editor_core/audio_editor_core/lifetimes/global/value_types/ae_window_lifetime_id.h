//
//  ae_window_lifetime_id.h
//

#pragma once

#include <audio_editor_core/ae_project_id.h>
#include <cpp_utils/yas_identifier.h>

namespace yas::ae {
struct window_lifetime_id final {
    identifier instance;
    project_id project;

    bool operator==(window_lifetime_id const &) const;
    bool operator!=(window_lifetime_id const &) const;
    bool operator<(window_lifetime_id const &) const;
};
}  // namespace yas::ae

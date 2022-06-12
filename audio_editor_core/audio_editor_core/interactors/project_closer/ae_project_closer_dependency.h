//
//  ae_project_closer_dependency.h
//

#pragma once

#include <audio_editor_core/ae_project_id.h>

#include <string>

namespace yas::ae {
struct file_importer_for_project_closer {
    virtual ~file_importer_for_project_closer() = default;

    virtual void cancel(project_id const &) = 0;
};

class project_id;

struct window_lifecycle_for_project_closer {
    virtual ~window_lifecycle_for_project_closer() = default;

    virtual void remove_lifetime(ae::project_id const &project_id) = 0;
};

enum class project_state;

struct project_state_holder_for_project_closer {
    virtual ~project_state_holder_for_project_closer() = default;

    [[nodiscard]] virtual project_state const &state() const = 0;
    virtual void set_state(project_state const &) = 0;
};
}  // namespace yas::ae

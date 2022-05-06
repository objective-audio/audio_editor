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

struct project_level_collector_for_project_closer {
    virtual ~project_level_collector_for_project_closer() = default;

    virtual void remove_level(ae::project_id const &project_id) = 0;
};

struct project_editor_level_pool_for_project_closer {
    virtual ~project_editor_level_pool_for_project_closer() = default;

    virtual void remove_level() = 0;
};

enum class project_state;

struct project_status_for_project_closer {
    virtual ~project_status_for_project_closer() = default;

    [[nodiscard]] virtual project_state const &state() const = 0;
    virtual void set_state(project_state const &) = 0;
};
}  // namespace yas::ae

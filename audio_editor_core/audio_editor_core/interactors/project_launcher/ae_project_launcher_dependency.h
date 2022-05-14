//
//  ae_project_launcher_dependency.h
//

#pragma once

#include <audio_editor_core/ae_edge_presenter_dependency.h>
#include <audio_editor_core/ae_file_importer_types.h>
#include <audio_editor_core/ae_file_info.h>
#include <audio_editor_core/ae_markers_presenter_dependency.h>
#include <audio_editor_core/ae_modules_presenter_dependency.h>
#include <audio_editor_core/ae_player_dependency.h>
#include <audio_editor_core/ae_project_format.h>
#include <audio_editor_core/ae_scroll_gesture_controller_dependency.h>
#include <cpp_utils/yas_identifier.h>
#include <cpp_utils/yas_url.h>

namespace yas::proc {
class timeline;
}

namespace yas::ae {
struct file_info_loader_for_project_launcher {
    virtual ~file_info_loader_for_project_launcher() = default;

    [[nodiscard]] virtual std::optional<file_info> load_file_info(url const &) const = 0;
};

class responder;

struct responder_stack_for_project_launcher {
    virtual ~responder_stack_for_project_launcher() = default;

    virtual void push_responder(identifier const &, std::shared_ptr<ae::responder> const &) = 0;
};

class project_editor_level;

struct project_editor_level_pool_for_project_launcher {
    virtual ~project_editor_level_pool_for_project_launcher() = default;

    virtual void add_level(file_info const &) = 0;

    [[nodiscard]] virtual std::shared_ptr<project_editor_level> const &level() const = 0;
};

enum class project_state;

struct project_state_holder_for_project_launcher {
    virtual ~project_state_holder_for_project_launcher() = default;

    [[nodiscard]] virtual project_state const &state() const = 0;
    virtual void set_state(project_state const &) = 0;
};
}  // namespace yas::ae

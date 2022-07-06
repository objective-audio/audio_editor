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

namespace yas::ae {
enum class project_state;

struct project_state_holder_for_project_launcher {
    virtual ~project_state_holder_for_project_launcher() = default;

    [[nodiscard]] virtual project_state const &state() const = 0;
    virtual void set_state(project_state const &) = 0;
};
}  // namespace yas::ae

//
//  ae_project_dependency.h
//

#pragma once

#include <audio_editor_core/ae_common_types.h>
#include <audio_editor_core/ae_edge_presenter_dependency.h>
#include <audio_editor_core/ae_editing_root_presenter_dependency.h>
#include <audio_editor_core/ae_file_importer_types.h>
#include <audio_editor_core/ae_file_info.h>
#include <audio_editor_core/ae_markers_presenter_dependency.h>
#include <audio_editor_core/ae_modules_presenter_dependency.h>
#include <audio_editor_core/ae_pinch_gesture_controller_dependency.h>
#include <audio_editor_core/ae_player_dependency.h>
#include <audio_editor_core/ae_project_editor_dependency.h>
#include <audio_editor_core/ae_scroll_gesture_controller_dependency.h>
#include <cpp_utils/yas_url.h>

namespace yas::proc {
class timeline;
}

namespace yas::ae {
struct project_url_for_project {
    virtual ~project_url_for_project() = default;

    [[nodiscard]] virtual url const &root_directory() const = 0;
    [[nodiscard]] virtual url editing_file() const = 0;
    [[nodiscard]] virtual url playing_directory() const = 0;
    [[nodiscard]] virtual url db_file() const = 0;
};

struct file_importer_for_project {
    virtual ~file_importer_for_project() = default;

    virtual void import(file_importing_context &&) = 0;
    virtual void cancel(std::string const &) = 0;
};

struct file_loader_for_project {
    virtual ~file_loader_for_project() = default;

    [[nodiscard]] virtual std::optional<file_info> load_file_info(url const &) const = 0;
};

struct scrolling_for_project : scrolling_for_player, scrolling_for_gesture_controller {
    virtual ~scrolling_for_project() = default;
};

class responder;

struct responder_stack_for_project {
    virtual ~responder_stack_for_project() = default;

    virtual void push_responder(identifier const &, std::shared_ptr<ae::responder> const &) = 0;
};

class project_editor_level;

struct project_editor_level_pool_for_project {
    virtual ~project_editor_level_pool_for_project() = default;

    virtual void add_level(file_info const &) = 0;
    virtual void remove_level() = 0;

    [[nodiscard]] virtual std::shared_ptr<project_editor_level> const &level() const = 0;
};
}  // namespace yas::ae

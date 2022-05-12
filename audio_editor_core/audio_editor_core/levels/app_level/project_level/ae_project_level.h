//
//  ae_project_level.h
//

#pragma once

#include <audio_editor_core/ae_project_id.h>
#include <cpp_utils/yas_url.h>

#include <memory>
#include <string>

namespace yas::ae {
class app_level;
class project_url;
class zooming_pair;
class scrolling;
class system_url;
class player;
class dialog_presenter;
class sheet_presenter;
class context_menu_presenter;
class responder_stack;
class project_status;
class project_closer;
class project_launcher;
class project_editor_level_pool;

struct project_level final {
    [[nodiscard]] static std::shared_ptr<project_level> make_shared(project_id const &, url const &file_url);

    project_level(ae::project_id const &, url const &file_url, std::shared_ptr<app_level> const &);

    ae::project_id const project_id;
    url const file_url;

    std::shared_ptr<ae::project_url> const project_url;
    std::shared_ptr<ae::zooming_pair> const zooming_pair;
    std::shared_ptr<ae::scrolling> const scrolling;
    std::shared_ptr<ae::player> const player;
    std::shared_ptr<ae::responder_stack> const responder_stack;
    std::shared_ptr<ae::dialog_presenter> const dialog_presenter;
    std::shared_ptr<ae::sheet_presenter> const sheet_presenter;
    std::shared_ptr<ae::context_menu_presenter> const context_menu_presenter;
    std::shared_ptr<ae::project_editor_level_pool> const editor_level_pool;
    std::shared_ptr<ae::project_status> const status;
    std::shared_ptr<ae::project_closer> const closer;
    std::shared_ptr<ae::project_launcher> const launcher;
};
}  // namespace yas::ae

//
//  ae_project_level.h
//

#pragma once

#include <cpp_utils/yas_url.h>

#include <memory>
#include <string>

namespace yas::ae {
class zooming;
class scrolling;
class system_url;
class player;
class action_router;
class action_controller;
class dialog_presenter;
class context_menu_presenter;
class project;

struct project_level final {
    [[nodiscard]] static std::shared_ptr<project_level> make_shared(std::string const &identifier, url const &file_url);

    std::string const identifier;

    std::shared_ptr<ae::zooming> const horizontal_zooming;
    std::shared_ptr<ae::zooming> const vertical_zooming;
    std::shared_ptr<ae::scrolling> const scrolling;
    std::shared_ptr<ae::player> const player;
    std::shared_ptr<ae::action_router> const action_router;
    std::shared_ptr<ae::action_controller> const action_controller;
    std::shared_ptr<ae::dialog_presenter> const dialog_presenter;
    std::shared_ptr<ae::context_menu_presenter> const context_menu_presenter;

    std::shared_ptr<project> const project;

   private:
    project_level(std::string const &identifier, std::shared_ptr<ae::system_url> const &,
                  std::shared_ptr<ae::scrolling> const &, std::shared_ptr<ae::player> const &,
                  std::shared_ptr<ae::action_router> const &, std::shared_ptr<ae::action_controller> const &,
                  std::shared_ptr<ae::dialog_presenter> const &, std::shared_ptr<ae::project> const &);
};
}  // namespace yas::ae

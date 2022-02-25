//
//  ae_project_editor_level_maker.h
//

#pragma once

#include <audio_editor_core/ae_action_controller.h>
#include <audio_editor_core/ae_dialog_presenter.h>
#include <audio_editor_core/ae_project_dependency.h>

namespace yas::ae {
class project_editor_level;

struct project_editor_level_maker final {
    [[nodiscard]] static std::shared_ptr<project_editor_level_maker> make_shared(
        std::string const &identifier, std::shared_ptr<player_for_project_editor> const &,
        std::shared_ptr<action_controller> const &, std::shared_ptr<dialog_presenter> const &);

    [[nodiscard]] std::shared_ptr<project_editor_level> make(file_info const &) const;

   private:
    std::string const _identifier;
    std::shared_ptr<player_for_project_editor> const _player;
    std::shared_ptr<action_controller> const _action_controller;
    std::shared_ptr<dialog_presenter> const _dialog_presenter;

    project_editor_level_maker(std::string const &identifier, std::shared_ptr<player_for_project_editor> const &,
                               std::shared_ptr<action_controller> const &, std::shared_ptr<dialog_presenter> const &);

    project_editor_level_maker(project_editor_level_maker const &) = delete;
    project_editor_level_maker(project_editor_level_maker &&) = delete;
    project_editor_level_maker &operator=(project_editor_level_maker const &) = delete;
    project_editor_level_maker &operator=(project_editor_level_maker &&) = delete;
};
}  // namespace yas::ae

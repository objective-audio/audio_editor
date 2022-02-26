//
//  ae_project_editor_level_pool.h
//

#pragma once

#include <audio_editor_core/ae_project_dependency.h>

#include <memory>

namespace yas::ae {
class project_editor_level;
class action_controller;
class dialog_presenter;

struct project_editor_level_pool final : project_editor_level_pool_for_project {
    [[nodiscard]] static std::shared_ptr<project_editor_level_pool> make_shared(
        std::string const &identifier, std::shared_ptr<player_for_project_editor> const &,
        std::shared_ptr<action_controller> const &, std::shared_ptr<dialog_presenter> const &);

    void add_editor_level(file_info const &) override;
    std::shared_ptr<project_editor_level> const &editor_level() const;

   private:
    std::string const _identifier;
    std::shared_ptr<player_for_project_editor> const _player;
    std::shared_ptr<action_controller> const _action_controller;
    std::shared_ptr<dialog_presenter> const _dialog_presenter;

    std::shared_ptr<project_editor_level> _editor_level;

    project_editor_level_pool(std::string const &identifier, std::shared_ptr<player_for_project_editor> const &,
                              std::shared_ptr<action_controller> const &, std::shared_ptr<dialog_presenter> const &);
};
}  // namespace yas::ae

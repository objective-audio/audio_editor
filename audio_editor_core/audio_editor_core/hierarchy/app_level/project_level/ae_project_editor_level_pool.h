//
//  ae_project_editor_level_pool.h
//

#pragma once

#include <audio_editor_core/ae_project_dependency.h>

#include <memory>

namespace yas::ae {
class project_editor_level;
class player;
class action_controller;
class dialog_presenter;

struct project_editor_level_pool final : project_editor_level_pool_for_project {
    [[nodiscard]] static std::shared_ptr<project_editor_level_pool> make_shared(std::string const &identifier);

    void add_editor_level(file_info const &) override;
    std::shared_ptr<project_editor_level> const &editor_level() const;

   private:
    std::string const _identifier;

    std::shared_ptr<project_editor_level> _editor_level;

    project_editor_level_pool(std::string const &identifier);
};
}  // namespace yas::ae

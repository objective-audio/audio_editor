//
//  ae_project_editor_maker.h
//

#pragma once

#include <audio_editor_core/ae_project_dependency.h>
#include <audio_editor_core/ae_project_editor_dependency.h>

namespace yas::ae {
struct project_editor_maker final : project_editor_maker_interface {
    [[nodiscard]] std::shared_ptr<project_editor_interface> make(url const &, file_info const &) const override;

    static std::shared_ptr<project_editor_maker> make_shared(std::shared_ptr<project_editor_player_interface> const &);

   private:
    std::shared_ptr<project_editor_player_interface> const _player;

    project_editor_maker(std::shared_ptr<project_editor_player_interface> const &);

    project_editor_maker(project_editor_maker const &) = delete;
    project_editor_maker(project_editor_maker &&) = delete;
    project_editor_maker &operator=(project_editor_maker const &) = delete;
    project_editor_maker &operator=(project_editor_maker &&) = delete;
};
}  // namespace yas::ae

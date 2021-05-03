//
//  ae_timeline_editor.h
//

#pragma once

#include <audio_editor_core/ae_project_dependency.h>
#include <audio_editor_core/ae_timeline_editor_dependency.h>

namespace yas::ae {
struct timeline_editor final : project_timeline_editor_interface {
    static std::shared_ptr<timeline_editor> make_shared(std::shared_ptr<timeline_player_interface> const &);

   private:
    std::shared_ptr<proc::timeline> const _timeline;
    std::shared_ptr<timeline_player_interface> const _player;

    timeline_editor(std::shared_ptr<timeline_player_interface> const &);

    timeline_editor(timeline_editor const &) = delete;
    timeline_editor(timeline_editor &&) = delete;
    timeline_editor &operator=(timeline_editor const &) = delete;
    timeline_editor &operator=(timeline_editor &&) = delete;
};
}  // namespace yas::ae

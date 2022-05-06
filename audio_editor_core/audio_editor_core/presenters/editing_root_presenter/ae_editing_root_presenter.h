//
//  ae_editing_root_presenter.h
//

#pragma once

#include <audio_editor_core/ae_action.h>
#include <audio_editor_core/ae_editing_root_presenter_types.h>
#include <audio_editor_core/ae_project_id.h>

namespace yas::ae {
class player;
class responder_stack;

struct editing_root_presenter final {
    [[nodiscard]] static std::shared_ptr<editing_root_presenter> make_shared(project_id const &project_id);

    [[nodiscard]] playing_line_state_t playing_line_state() const;

    [[nodiscard]] bool responds_to_action(action const);

   private:
    std::weak_ptr<player> const _player;
    std::weak_ptr<responder_stack> const _responder_stack;

    editing_root_presenter(std::shared_ptr<player> const &, std::shared_ptr<responder_stack> const &);

    editing_root_presenter(editing_root_presenter const &) = delete;
    editing_root_presenter(editing_root_presenter &&) = delete;
    editing_root_presenter &operator=(editing_root_presenter const &) = delete;
    editing_root_presenter &operator=(editing_root_presenter &&) = delete;
};
}  // namespace yas::ae

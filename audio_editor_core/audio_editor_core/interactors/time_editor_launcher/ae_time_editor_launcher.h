//
//  ae_time_editor_launcher.h
//

#pragma once

#include <audio_editor_core/ae_project_id.h>

#include <memory>
#include <optional>

namespace yas::ae {
class player;
class timing;
class project_sub_level_router;
class responder_stack;

struct time_editor_launcher final {
    [[nodiscard]] static std::shared_ptr<time_editor_launcher> make_shared(player const *, timing const *,
                                                                           project_sub_level_router *,
                                                                           responder_stack *);

    time_editor_launcher(player const *, timing const *, project_sub_level_router *, responder_stack *);

    bool can_begin_time_editing() const;
    void begin_time_editing(std::optional<std::size_t> const unit_idx);

   private:
    player const *const _player;
    timing const *const _timing;
    project_sub_level_router *const _router;
    responder_stack *const _responder_stack;
};
}  // namespace yas::ae

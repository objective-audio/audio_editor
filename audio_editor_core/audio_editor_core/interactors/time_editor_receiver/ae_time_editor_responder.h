//
//  ae_time_editor_responder.h
//

#pragma once

#include <audio_editor_core/ae_responder.h>

namespace yas::ae {
class time_editor;
class time_editor_closer;

struct time_editor_responder final : responder {
    [[nodiscard]] static std::shared_ptr<time_editor_responder> make_shared(project_id const &, time_editor *,
                                                                            time_editor_closer *);

    time_editor_responder(project_id const &, time_editor *, time_editor_closer *);

    std::optional<ae::action> to_action(ae::key const &) override;
    void handle_action(ae::action const &) override;
    [[nodiscard]] responding responding_to_action(ae::action const &) override;

   private:
    project_id const _project_id;
    identifier const _responder_id;
    time_editor *_editor;
    time_editor_closer *_closer;
};
}  // namespace yas::ae

//
//  ae_project_editor_responder.h
//

#pragma once

#include <audio_editor_core/ae_responder.h>

namespace yas::ae {
class project_editor;
class playing_toggler;
class nudge_settings;
class nudger;
class jumper;
class edge_editor;
class time_editor_launcher;
class marker_editor;

struct project_editor_responder final : responder {
    [[nodiscard]] static std::shared_ptr<project_editor_responder> make_shared(project_editor *, playing_toggler *,
                                                                               nudge_settings *, nudger *, jumper *,
                                                                               edge_editor *, time_editor_launcher *,
                                                                               marker_editor *);

    std::optional<ae::action> to_action(ae::key const &) override;
    void handle_action(ae::action const &) override;
    [[nodiscard]] responding responding_to_action(ae::action const &) override;

   private:
    identifier const _responder_id;
    project_editor *const _editor;
    playing_toggler *const _playing_toggler;
    nudge_settings *const _nudge_settings;
    nudger *const _nudger;
    jumper *const _jumper;
    edge_editor *const _edge_editor;
    time_editor_launcher *const _time_editor_launcher;
    marker_editor *const _marker_editor;

    project_editor_responder(project_editor *, playing_toggler *, nudge_settings *, nudger *, jumper *, edge_editor *,
                             time_editor_launcher *, marker_editor *);
};
}  // namespace yas::ae

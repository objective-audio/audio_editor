//
//  ae_project_receiver.h
//

#pragma once

#include <audio_editor_core/ae_action_receivable.h>
#include <audio_editor_core/ae_window_lifetime_id.h>

namespace yas::ae {
class track_editor;
class playing_toggler;
class nudge_settings;
class nudger;
class jumper;
class edge_editor;
class time_editor_launcher;
class marker_editor;
class module_renaming_launcher;
class timing;
class import_interactor;
class export_interactor;
class reverter;

struct project_receiver final : action_receivable {
    [[nodiscard]] static std::shared_ptr<project_receiver> make_shared(window_lifetime_id const &, track_editor *,
                                                                       playing_toggler *, nudge_settings *, nudger *,
                                                                       jumper *, edge_editor *, time_editor_launcher *,
                                                                       marker_editor *, module_renaming_launcher *,
                                                                       timing *, import_interactor *,
                                                                       export_interactor *, reverter *);

    project_receiver(window_lifetime_id const &, track_editor *, playing_toggler *, nudge_settings *, nudger *,
                     jumper *, edge_editor *, time_editor_launcher *, marker_editor *, module_renaming_launcher *,
                     timing *, import_interactor *, export_interactor *, reverter *);

   private:
    window_lifetime_id const _window_lifetime_id;
    track_editor *const _editor;
    playing_toggler *const _playing_toggler;
    nudge_settings *const _nudge_settings;
    nudger *const _nudger;
    jumper *const _jumper;
    edge_editor *const _edge_editor;
    time_editor_launcher *const _time_editor_launcher;
    marker_editor *const _marker_editor;
    module_renaming_launcher *const _module_renaming_launcher;
    timing *_timing;
    import_interactor *_import_interactor;
    export_interactor *_export_interactor;
    reverter *_reverter;

    std::optional<action_id> receivable_id() const override;
    std::optional<ae::action> to_action(ae::key const &, ae::action_id const &) const override;
    void handle_action(ae::action const &) const override;
    responding responding_to_action(ae::action const &) const override;
};
}  // namespace yas::ae

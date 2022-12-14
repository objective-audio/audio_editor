//
//  ae_project_receiver.h
//

#pragma once

#include <audio_editor_core/ae_action_receivable.h>
#include <audio_editor_core/ae_window_lifetime_id.h>

namespace yas::ae {
class database;
class track_editor;
class playing_toggler;
class nudge_settings;
class nudger;
class jumper;
class edge_editor;
class time_editor_opener;
class marker_editor;
class module_renaming_opener;
class marker_renaming_opener;
class timing;
class import_interactor;
class export_interactor;
class reverter;
class module_selector;
class marker_selector;
class escaper;
class pasteboard;

struct project_receiver final : action_receivable {
    project_receiver(window_lifetime_id const &, database *, track_editor *, playing_toggler *, nudge_settings *,
                     nudger *, jumper *, edge_editor *, time_editor_opener *, marker_editor *, module_renaming_opener *,
                     marker_renaming_opener *, timing *, import_interactor *, export_interactor *, reverter *,
                     module_selector *, marker_selector *, escaper *, pasteboard *);

   private:
    window_lifetime_id const _window_lifetime_id;
    database *const _database;
    track_editor *const _track_editor;
    playing_toggler *const _playing_toggler;
    nudge_settings *const _nudge_settings;
    nudger *const _nudger;
    jumper *const _jumper;
    edge_editor *const _edge_editor;
    time_editor_opener *const _time_editor_opener;
    marker_editor *const _marker_editor;
    module_renaming_opener *const _module_renaming_opener;
    marker_renaming_opener *const _marker_renaming_opener;
    timing *const _timing;
    import_interactor *const _import_interactor;
    export_interactor *const _export_interactor;
    reverter *const _reverter;
    module_selector *const _module_selector;
    marker_selector *const _marker_selector;
    escaper *const _escaper;
    pasteboard *const _pasteboard;

    project_receiver(project_receiver const &) = delete;
    project_receiver(project_receiver &&) = delete;
    project_receiver &operator=(project_receiver const &) = delete;
    project_receiver &operator=(project_receiver &&) = delete;

    std::optional<action_id> receivable_id() const override;
    std::optional<ae::action> to_action(ae::key const &) const override;
    void receive(ae::action const &) const override;
    action_receivable_state receivable_state(ae::action const &) const override;
};
}  // namespace yas::ae

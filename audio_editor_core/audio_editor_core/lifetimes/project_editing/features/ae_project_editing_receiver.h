//
//  ae_project_editing_receiver.h
//

#pragma once

#include <audio_editor_core/ae_action_receivable.h>
#include <audio_editor_core/ae_project_lifetime_id.h>

namespace yas::ae {
class database;
class module_editor;
class playing_toggler;
class nudging;
class nudger;
class jumper;
class edge_editor;
class time_editing_opener;
class marker_editor;
class module_renaming_opener;
class marker_renaming_opener;
class project_settings_opener;
class timing;
class import_interactor;
class export_interactor;
class reverter;
class module_selector;
class marker_selector;
class track_selector;
class escaper;
class pasteboard;
class vertical_scrolling;

struct project_editing_receiver final : action_receivable {
    project_editing_receiver(project_lifetime_id const &, database *, module_editor *, playing_toggler *, nudging *,
                             nudger *, jumper *, edge_editor *, time_editing_opener *, marker_editor *,
                             module_renaming_opener *, marker_renaming_opener *, project_settings_opener *, timing *,
                             import_interactor *, export_interactor *, reverter *, module_selector *, marker_selector *,
                             track_selector *, escaper *, pasteboard *, vertical_scrolling *);

   private:
    project_lifetime_id const _project_lifetime_id;
    database *const _database;
    module_editor *const _module_editor;
    playing_toggler *const _playing_toggler;
    nudging *const _nudging;
    nudger *const _nudger;
    jumper *const _jumper;
    edge_editor *const _edge_editor;
    time_editing_opener *const _time_editing_opener;
    marker_editor *const _marker_editor;
    module_renaming_opener *const _module_renaming_opener;
    marker_renaming_opener *const _marker_renaming_opener;
    project_settings_opener *const _settings_opener;
    timing *const _timing;
    import_interactor *const _import_interactor;
    export_interactor *const _export_interactor;
    reverter *const _reverter;
    module_selector *const _module_selector;
    marker_selector *const _marker_selector;
    track_selector *const _track_selector;
    escaper *const _escaper;
    pasteboard *const _pasteboard;
    vertical_scrolling *const _vertical_scrolling;

    project_editing_receiver(project_editing_receiver const &) = delete;
    project_editing_receiver(project_editing_receiver &&) = delete;
    project_editing_receiver &operator=(project_editing_receiver const &) = delete;
    project_editing_receiver &operator=(project_editing_receiver &&) = delete;

    std::optional<action_id> receivable_id() const override;
    std::optional<ae::action> to_action(ae::key const &) const override;
    void receive(ae::action const &) const override;
    action_receivable_state receivable_state(ae::action const &) const override;
};
}  // namespace yas::ae

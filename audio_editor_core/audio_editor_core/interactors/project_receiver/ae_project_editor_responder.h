//
//  ae_project_editor_responder.h
//

#pragma once

#include <audio_editor_core/ae_action.h>
#include <audio_editor_core/ae_keyboard_types.h>
#include <audio_editor_core/ae_responding.h>
#include <cpp_utils/yas_identifier.h>

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

struct project_editor_responder final {
    [[nodiscard]] static std::shared_ptr<project_editor_responder> make_shared(
        project_id const &, track_editor *, playing_toggler *, nudge_settings *, nudger *, jumper *, edge_editor *,
        time_editor_launcher *, marker_editor *, module_renaming_launcher *, timing *, import_interactor *,
        export_interactor *, reverter *);

    project_editor_responder(project_id const &, track_editor *, playing_toggler *, nudge_settings *, nudger *,
                             jumper *, edge_editor *, time_editor_launcher *, marker_editor *,
                             module_renaming_launcher *, timing *, import_interactor *, export_interactor *,
                             reverter *);

    std::optional<ae::action> to_action(ae::key const &);
    void handle_action(ae::action const &);
    [[nodiscard]] responding responding_to_action(ae::action const &);

   private:
    project_id const _project_id;
    identifier const _responder_id;
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
};
}  // namespace yas::ae

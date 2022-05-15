//
//  ae_export_interactor.h
//

#pragma once

#include <audio_editor_core/ae_common_types.h>
#include <audio_editor_core/ae_project_format.h>
#include <audio_editor_core/ae_project_id.h>
#include <cpp_utils/yas_url.h>

#include <memory>

namespace yas::ae {
class dialog_presenter;
class editing_status;
class edge_holder;
class player;
class exporter;
class timeline_holder;

struct export_interactor final {
    [[nodiscard]] static std::shared_ptr<export_interactor> make_shared(project_id const &, project_format const &,
                                                                        editing_status const *, edge_holder const *,
                                                                        exporter *, timeline_holder const *);

    export_interactor(project_format const &, dialog_presenter *, editing_status const *, edge_holder const *, player *,
                      exporter *, timeline_holder const *);

    [[nodiscard]] bool can_select_file_for_export() const;
    void select_file_for_export();
    [[nodiscard]] bool can_export_to_file() const;
    void export_to_file(url const &);

   private:
    project_format const _project_format;
    dialog_presenter *const _dialog_presenter;
    editing_status const *const _editing_status;
    edge_holder const *const _edge_holder;
    player *const _player;
    exporter *const _exporter;
    timeline_holder const *const _timeline_holder;
};
}  // namespace yas::ae

//
//  ae_export_interactor.h
//

#pragma once

#include <audio_editor_core/ae_file_info.h>
#include <audio_editor_core/ae_project_id.h>
#include <cpp_utils/yas_url.h>

#include <memory>

namespace yas::ae {
class dialog_presenter;
class editing_status;
class edge_holder;
class player;
class exporter;
class timeline_updater;

struct export_interactor final {
    [[nodiscard]] static std::shared_ptr<export_interactor> make_shared(project_id const &, file_info const &,
                                                                        editing_status const *, edge_holder const *,
                                                                        exporter *, timeline_updater const *);

    export_interactor(file_info const &, dialog_presenter *, editing_status const *, edge_holder const *, player *,
                      exporter *, timeline_updater const *);

    [[nodiscard]] bool can_select_file_for_export() const;
    void select_file_for_export();
    [[nodiscard]] bool can_export_to_file() const;
    void export_to_file(url const &);

   private:
    file_info const _file_info;
    dialog_presenter *const _dialog_presenter;
    editing_status const *const _editing_status;
    edge_holder const *const _edge_holder;
    player *const _player;
    exporter *const _exporter;
    timeline_updater const *const _timeline_updater;
};
}  // namespace yas::ae

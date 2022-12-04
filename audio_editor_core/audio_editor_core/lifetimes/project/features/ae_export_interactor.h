//
//  ae_export_interactor.h
//

#pragma once

#include <audio_editor_core/ae_common_types.h>
#include <audio_editor_core/ae_project_format.h>

#include <filesystem>
#include <memory>

namespace yas::ae {
class project_modal_lifecycle;
class editing_status;
class edge_holder;
class player;
class exporter;
class timeline_holder;

struct export_interactor final {
    export_interactor(project_format const &, project_modal_lifecycle *, editing_status const *, edge_holder const *,
                      player *, exporter *, timeline_holder const *);

    [[nodiscard]] bool can_select_file_for_export() const;
    void select_file_for_export();
    [[nodiscard]] bool can_export_to_file() const;
    void export_to_file(std::filesystem::path const &);

   private:
    project_format const _project_format;
    project_modal_lifecycle *const _lifecycle;
    editing_status const *const _editing_status;
    edge_holder const *const _edge_holder;
    player *const _player;
    exporter *const _exporter;
    timeline_holder const *const _timeline_holder;
};
}  // namespace yas::ae

//
//  export_interactor.h
//

#pragma once

#include <ae-core/app/value_types/project_format.h>
#include <ae-core/global/value_types/common_types.h>

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

    export_interactor(export_interactor const &) = delete;
    export_interactor(export_interactor &&) = delete;
    export_interactor &operator=(export_interactor const &) = delete;
    export_interactor &operator=(export_interactor &&) = delete;
};
}  // namespace yas::ae

//
//  ae_project_closer.h
//

#pragma once

#include <audio_editor_core/ae_project_closer_dependency.h>
#include <audio_editor_core/ae_project_id.h>
#include <audio_editor_core/ae_project_state.h>
#include <audio_editor_core/ae_window_presenter_dependency.h>
#include <observing/yas_observing_umbrella.h>

namespace yas::ae {
class file_importer;
class project_state_holder;

struct project_closer final {
    [[nodiscard]] static std::shared_ptr<project_closer> make_shared(project_id const &project_id,
                                                                     file_importer_for_project_closer *,
                                                                     window_lifecycle_for_project_closer *,
                                                                     project_state_holder_for_project_closer *);

    project_closer(project_id const &project_id, file_importer_for_project_closer *,
                   window_lifecycle_for_project_closer *, project_state_holder_for_project_closer *);

    [[nodiscard]] bool can_close() const;
    void request_close();

   private:
    project_id const _project_id;

    file_importer_for_project_closer *const _file_importer;
    window_lifecycle_for_project_closer *const _window_lifecycle;
    project_state_holder_for_project_closer *const _status;
};
}  // namespace yas::ae

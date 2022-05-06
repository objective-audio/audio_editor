//
//  ae_project_launcher.h
//

#pragma once

#include <audio_editor_core/ae_project_id.h>
#include <audio_editor_core/ae_project_launcher_dependency.h>
#include <audio_editor_core/ae_project_state.h>
#include <audio_editor_core/ae_window_presenter_dependency.h>

namespace yas::ae {
struct project_launcher final {
    [[nodiscard]] static std::shared_ptr<project_launcher> make_shared(project_id const &, url const &file_url,
                                                                       project_url_for_project_launcher const *,
                                                                       file_importer_for_project_launcher *,
                                                                       file_loader_for_project_launcher *,
                                                                       responder_stack_for_project_launcher *,
                                                                       project_editor_level_pool_for_project_launcher *,
                                                                       project_status_for_project_launcher *);

    void launch();

   private:
    std::weak_ptr<project_launcher> _weak_launcher;

    project_id const _project_id;
    url const _file_url;

    project_url_for_project_launcher const *const _project_url;
    file_importer_for_project_launcher *const _file_importer;
    file_loader_for_project_launcher *const _file_loader;
    responder_stack_for_project_launcher *const _responder_stack;
    project_editor_level_pool_for_project_launcher *const _editor_level_pool;
    project_status_for_project_launcher *const _status;

    observing::canceller_pool _pool;

    project_launcher(project_id const &, url const &file_url, project_url_for_project_launcher const *,
                     file_importer_for_project_launcher *, file_loader_for_project_launcher *,
                     responder_stack_for_project_launcher *, project_editor_level_pool_for_project_launcher *,
                     project_status_for_project_launcher *);

    project_launcher(project_launcher const &) = delete;
    project_launcher(project_launcher &&) = delete;
    project_launcher &operator=(project_launcher const &) = delete;
    project_launcher &operator=(project_launcher &&) = delete;
};
}  // namespace yas::ae

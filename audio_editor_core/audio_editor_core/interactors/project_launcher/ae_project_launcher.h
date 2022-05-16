//
//  ae_project_launcher.h
//

#pragma once

#include <audio_editor_core/ae_project_id.h>
#include <audio_editor_core/ae_project_launcher_dependency.h>
#include <audio_editor_core/ae_project_state.h>
#include <audio_editor_core/ae_window_presenter_dependency.h>
#include <cpp_utils/yas_identifier.h>

namespace yas::ae {
class project_editor_responder;

struct project_launcher final : std::enable_shared_from_this<project_launcher> {
    [[nodiscard]] static std::shared_ptr<project_launcher> make_shared(
        identifier const &instance_id, url const &file_url, file_info_loader_for_project_launcher const *,
        responder_stack_for_project_launcher *, project_state_holder_for_project_launcher *,
        std::shared_ptr<project_editor_responder> const &);

    project_launcher(identifier const &instance_id, url const &file_url, file_info_loader_for_project_launcher const *,
                     responder_stack_for_project_launcher *, project_state_holder_for_project_launcher *,
                     std::shared_ptr<project_editor_responder> const &);

    void launch();

   private:
    identifier const _instance_id;
    url const _file_url;

    file_info_loader_for_project_launcher const *const _file_info_loader;
    responder_stack_for_project_launcher *const _responder_stack;
    project_state_holder_for_project_launcher *const _state_holder;
    std::weak_ptr<project_editor_responder> const _responder;

    observing::canceller_pool _pool;

    project_launcher(project_launcher const &) = delete;
    project_launcher(project_launcher &&) = delete;
    project_launcher &operator=(project_launcher const &) = delete;
    project_launcher &operator=(project_launcher &&) = delete;
};
}  // namespace yas::ae

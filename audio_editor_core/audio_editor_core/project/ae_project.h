//
//  ae_project.h
//

#pragma once

#include <audio_editor_core/ae_editing_root_presenter_dependency.h>
#include <audio_editor_core/ae_project_dependency.h>
#include <audio_editor_core/ae_project_types.h>
#include <audio_editor_core/ae_window_presenter_dependency.h>

namespace yas::ae {
class responder_stack;

struct project final : project_for_window_presenter {
    [[nodiscard]] static std::shared_ptr<project> make_shared(
        std::string const &identifier, url const &file_url, std::shared_ptr<project_url_for_project> const &,
        std::shared_ptr<file_importer_for_project> const &, std::shared_ptr<file_loader_for_project> const &,
        std::shared_ptr<responder_stack_for_project> const &,
        std::shared_ptr<project_editor_level_pool_for_project> const &);

    void setup();

    [[nodiscard]] project_state const &state() const;

    [[nodiscard]] bool can_close() const override;
    void request_close() override;

    [[nodiscard]] observing::syncable observe_state(std::function<void(project_state const &)> &&);
    [[nodiscard]] observing::endable observe_event(std::function<void(project_event const &)> &&);

   private:
    std::weak_ptr<project> _weak_project;

    std::string const _identifier;
    url const _file_url;

    std::shared_ptr<project_url_for_project> const _project_url;
    std::shared_ptr<file_importer_for_project> const _file_importer;
    std::shared_ptr<file_loader_for_project> const _file_loader;
    std::weak_ptr<responder_stack_for_project> const _responder_stack;
    std::shared_ptr<project_editor_level_pool_for_project> const _editor_level_pool;

    observing::value::holder_ptr<project_state> const _state;

    observing::notifier_ptr<project_event> const _event_notifier;
    observing::canceller_pool _pool;

    project(std::string const &identifier, url const &file_url, std::shared_ptr<project_url_for_project> const &,
            std::shared_ptr<file_importer_for_project> const &, std::shared_ptr<file_loader_for_project> const &,
            std::shared_ptr<responder_stack_for_project> const &,
            std::shared_ptr<project_editor_level_pool_for_project> const &);

    project(project const &) = delete;
    project(project &&) = delete;
    project &operator=(project const &) = delete;
    project &operator=(project &&) = delete;
};
}  // namespace yas::ae

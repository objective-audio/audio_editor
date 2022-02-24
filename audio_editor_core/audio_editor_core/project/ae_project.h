//
//  ae_project.h
//

#pragma once

#include <audio_editor_core/ae_project_dependency.h>
#include <audio_editor_core/ae_project_pool_dependency.h>

namespace yas::ae {
class zooming;

struct project final : project_for_window_presenter, project_for_root_presenter, project_for_editing_root_presenter {
    [[nodiscard]] static std::shared_ptr<project> make_shared(
        std::string const &identifier, url const &file_url, std::shared_ptr<project_url_for_project> const &,
        std::shared_ptr<file_importer_for_project> const &, std::shared_ptr<file_loader_for_project> const &,
        std::shared_ptr<player_for_project> const &, std::shared_ptr<project_editor_maker_for_project> const &,
        std::shared_ptr<scrolling_for_project> const &, std::shared_ptr<ae::action_controller> const &,
        std::shared_ptr<ae::dialog_presenter> const &, std::shared_ptr<ae::context_menu_presenter> const &,
        std::shared_ptr<ae::action_router> const &);

    [[nodiscard]] std::string const &identifier() const override;
    [[nodiscard]] url const &file_url() const override;
    [[nodiscard]] project_state const &state() const override;

    std::shared_ptr<project_url_for_project> const project_url;
    std::shared_ptr<player_for_project> const player;

    std::shared_ptr<project_editor_for_project> editor;

    std::shared_ptr<scrolling_for_project> const scrolling;
    std::shared_ptr<ae::action_controller> const action_controller;
    std::shared_ptr<ae::dialog_presenter> const dialog_presenter;
    std::shared_ptr<ae::context_menu_presenter> const context_menu_presenter;
    std::shared_ptr<ae::action_router> const action_router;

    [[nodiscard]] bool can_close() const override;
    void request_close() override;

    [[nodiscard]] observing::syncable observe_state(std::function<void(project_state const &)> &&) override;
    [[nodiscard]] observing::endable observe_event(std::function<void(project_event const &)> &&);

   private:
    std::string const _identifier;
    url const _file_url;

    std::shared_ptr<file_importer_for_project> const _file_importer;
    std::shared_ptr<file_loader_for_project> const _file_loader;
    std::shared_ptr<project_editor_maker_for_project> const _editor_maker;

    observing::value::holder_ptr<project_state> const _state;

    observing::notifier_ptr<project_event> const _event_notifier;
    observing::canceller_pool _pool;

    project(std::string const &identifier, url const &file_url, std::shared_ptr<project_url_for_project> const &,
            std::shared_ptr<file_importer_for_project> const &, std::shared_ptr<file_loader_for_project> const &,
            std::shared_ptr<player_for_project> const &, std::shared_ptr<project_editor_maker_for_project> const &,
            std::shared_ptr<scrolling_for_project> const &, std::shared_ptr<ae::action_controller> const &,
            std::shared_ptr<ae::dialog_presenter> const &, std::shared_ptr<ae::context_menu_presenter> const &,
            std::shared_ptr<ae::action_router> const &);

    project(project const &) = delete;
    project(project &&) = delete;
    project &operator=(project const &) = delete;
    project &operator=(project &&) = delete;

    void _setup(std::weak_ptr<project>);
};
}  // namespace yas::ae

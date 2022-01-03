//
//  ae_project_pool_dependency.h
//

#pragma once

#include <audio_editor_core/ae_action_controller.h>
#include <audio_editor_core/ae_dialog_presenter.h>
#include <audio_editor_core/ae_editing_root_presenter_dependency.h>
#include <audio_editor_core/ae_project_dependency.h>
#include <audio_editor_core/ae_root_presenter_dependency.h>
#include <audio_editor_core/ae_window_presenter_dependency.h>

namespace yas::ae {
struct project_for_project_pool : project_for_window_presenter,
                                  project_for_root_presenter,
                                  project_for_editing_root_presenter {
    virtual ~project_for_project_pool() = default;

    [[nodiscard]] virtual std::string const &identifier() const = 0;
    [[nodiscard]] virtual std::shared_ptr<project_url_for_project> const &project_url() const = 0;
    [[nodiscard]] virtual std::shared_ptr<project_editor_for_project> const &editor() const = 0;
    [[nodiscard]] virtual std::shared_ptr<zooming_for_project> const &horizontal_zooming() const = 0;
    [[nodiscard]] virtual std::shared_ptr<scrolling_for_project> const &scrolling() const = 0;
    [[nodiscard]] virtual std::shared_ptr<action_controller> const &action_controller() const = 0;
    [[nodiscard]] virtual std::shared_ptr<dialog_presenter> const &dialog_presenter() const = 0;

    [[nodiscard]] virtual observing::endable observe_event(std::function<void(project_event const &)> &&) = 0;
};

struct project_maker_for_project_pool {
    virtual ~project_maker_for_project_pool() = default;

    [[nodiscard]] virtual std::shared_ptr<project_for_project_pool> make(url const &file_url) = 0;
};
}  // namespace yas::ae

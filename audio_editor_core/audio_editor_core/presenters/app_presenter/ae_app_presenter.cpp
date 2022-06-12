//
//  ae_app_presenter.cpp
//

#include "ae_app_presenter.h"

#include <audio_editor_core/ae_app_dialog_lifecycle.h>
#include <audio_editor_core/ae_app_lifetime.h>
#include <audio_editor_core/ae_app_presenter_utils.h>
#include <audio_editor_core/ae_hierarchy.h>
#include <audio_editor_core/ae_project_preparer.h>
#include <audio_editor_core/ae_window_lifecycle.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<app_presenter> app_presenter::make_shared() {
    return std::make_shared<app_presenter>(hierarchy::app_lifetime()->window_lifecycle,
                                           hierarchy::app_lifetime()->dialog_lifecycle,
                                           hierarchy::app_lifetime()->project_preparer);
}

app_presenter::app_presenter(std::shared_ptr<window_lifecycle_for_app_presenter> const &window_lifecycle,
                             std::shared_ptr<app_dialog_lifecycle> const &dialog_lifecycle,
                             std::shared_ptr<project_preparer> const &preparer)
    : _window_lifecycle(window_lifecycle), _dialog_lifecycle(dialog_lifecycle), _project_preparer(preparer) {
}

bool app_presenter::can_open_dialog() const {
    if (auto const lifecycle = this->_dialog_lifecycle.lock()) {
        return !lifecycle->current().has_value();
    } else {
        return false;
    }
}

void app_presenter::open_project_setup_dialog() {
    if (auto const lifecycle = this->_dialog_lifecycle.lock()) {
        lifecycle->add_project_format_dialog();
    }
}

observing::syncable app_presenter::observe_window(std::function<void(app_presenter_window_event const &)> &&handler) {
    if (auto const lifecycle = this->_window_lifecycle.lock()) {
        return lifecycle->observe_event([handler = std::move(handler)](window_lifecycle_event const &event) {
            handler(app_presenter_window_event{.type = to_presenter_event_type(event.type),
                                               .project_id = event.project_id});
        });
    } else {
        return observing::syncable{};
    }
}

observing::syncable app_presenter::observe_dialog(
    std::function<void(std::optional<app_dialog_sub_lifetime> const &)> &&handler) {
    if (auto const lifecycle = this->_dialog_lifecycle.lock()) {
        return lifecycle->observe(std::move(handler));
    } else {
        return observing::syncable{};
    }
}

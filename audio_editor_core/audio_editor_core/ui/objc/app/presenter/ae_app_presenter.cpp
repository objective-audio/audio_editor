//
//  ae_app_presenter.cpp
//

#include "ae_app_presenter.h"

#include <audio_editor_core/ae_app_lifetime.h>
#include <audio_editor_core/ae_app_modal_lifecycle.h>
#include <audio_editor_core/ae_app_presenter_utils.h>
#include <audio_editor_core/ae_hierarchy.h>
#include <audio_editor_core/ae_window_lifecycle.h>
#include <audio_editor_core/ae_window_opener.h>

#include <audio_editor_core/ae_app_settings_lifecycle.hpp>
#include <audio_editor_core/ae_project_settings_lifecycle.hpp>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<app_presenter> app_presenter::make_shared() {
    auto const &app_lifetime = hierarchy::app_lifetime();
    return std::make_shared<app_presenter>(app_lifetime->window_lifecycle, app_lifetime->project_settings_lifecycle,
                                           app_lifetime->app_settings_lifecycle, app_lifetime->modal_lifecycle,
                                           app_lifetime->window_opener);
}

app_presenter::app_presenter(
    std::shared_ptr<window_lifecycle_for_app_presenter> const &window_lifecycle,
    std::shared_ptr<project_settings_lifecycle_for_app_presenter> const &project_settings_lifecycle,
    std::shared_ptr<app_settings_lifecycle_for_app_presenter> const &app_settings_lifecycle,
    std::shared_ptr<app_modal_lifecycle> const &app_modal_lifecycle, std::shared_ptr<window_opener> const &opener)
    : _window_lifecycle(window_lifecycle),
      _project_settings_lifecycle(project_settings_lifecycle),
      _app_settings_lifecycle(app_settings_lifecycle),
      _app_modal_lifecycle(app_modal_lifecycle),
      _window_opener(opener) {
}

bool app_presenter::can_open_dialog() const {
    if (auto const lifecycle = this->_app_modal_lifecycle.lock()) {
        return !lifecycle->current().has_value();
    } else {
        return false;
    }
}

void app_presenter::open_project_setup_dialog() {
    if (auto const lifecycle = this->_app_modal_lifecycle.lock()) {
        lifecycle->add_project_setup_dialog();
    }
}

bool app_presenter::can_open_app_settings() const {
    if (auto const lifecycle = this->_app_settings_lifecycle.lock()) {
        return !lifecycle->has_current();
    } else {
        return false;
    }
}

void app_presenter::open_app_settings() {
    if (auto const lifecycle = this->_app_settings_lifecycle.lock()) {
        lifecycle->add_lifetime();
    }
}

observing::syncable app_presenter::observe_window(std::function<void(app_presenter_window_event const &)> &&handler) {
    observing::syncable result{};

    if (auto const lifecycle = this->_window_lifecycle.lock()) {
        auto syncable = lifecycle->observe_event([handler](window_lifecycle_event const &event) {
            handler(
                app_presenter_window_event{.type = to_presenter_event_type(event.type), .value = event.lifetime_id});
        });

        result.merge(std::move(syncable));
    }

    if (auto const lifecycle = this->_project_settings_lifecycle.lock()) {
        auto syncable = lifecycle->observe_event([handler](project_settings_lifecycle_event const &event) {
            handler(
                app_presenter_window_event{.type = to_presenter_event_type(event.type), .value = event.lifetime_id});
        });

        result.merge(std::move(syncable));
    }

    if (auto const lifecycle = this->_app_settings_lifecycle.lock()) {
        auto syncable = lifecycle->observe_event([handler](app_settings_lifecycle_event const &event) {
            handler(
                app_presenter_window_event{.type = to_presenter_event_type(event.type), .value = event.lifetime_id});
        });

        result.merge(std::move(syncable));
    }

    return result;
}

observing::syncable app_presenter::observe_dialog(
    std::function<void(std::optional<app_modal_sub_lifetime> const &)> &&handler) {
    if (auto const lifecycle = this->_app_modal_lifecycle.lock()) {
        return lifecycle->observe(std::move(handler));
    } else {
        return observing::syncable{};
    }
}

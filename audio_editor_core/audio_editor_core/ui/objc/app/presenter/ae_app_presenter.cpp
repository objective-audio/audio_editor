//
//  ae_app_presenter.cpp
//

#include "ae_app_presenter.h"

#include <audio_editor_core/ae_app_lifetime.h>
#include <audio_editor_core/ae_app_modal_lifecycle.h>
#include <audio_editor_core/ae_app_presenter_utils.h>
#include <audio_editor_core/ae_hierarchy.h>
#include <audio_editor_core/ae_project_lifecycle.h>
#include <audio_editor_core/ae_project_opener.h>

#include <audio_editor_core/ae_app_settings_lifecycle.hpp>
#include <audio_editor_core/ae_app_settings_opener.hpp>
#include <audio_editor_core/ae_project_settings_lifecycle.hpp>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<app_presenter> app_presenter::make_shared() {
    auto const &app_lifetime = hierarchy::app_lifetime();
    return std::make_shared<app_presenter>(app_lifetime->project_lifecycle, app_lifetime->project_settings_lifecycle,
                                           app_lifetime->app_settings_lifecycle, app_lifetime->modal_lifecycle,
                                           app_lifetime->project_opener, app_lifetime->app_settings_opener);
}

app_presenter::app_presenter(std::shared_ptr<project_lifecycle> const &project_lifecycle,
                             std::shared_ptr<project_settings_lifecycle> const &project_settings_lifecycle,
                             std::shared_ptr<app_settings_lifecycle> const &app_settings_lifecycle,
                             std::shared_ptr<app_modal_lifecycle> const &app_modal_lifecycle,
                             std::shared_ptr<project_opener> const &opener,
                             std::shared_ptr<app_settings_opener> const &app_settings_opener)
    : _project_lifecycle(project_lifecycle),
      _project_settings_lifecycle(project_settings_lifecycle),
      _app_settings_lifecycle(app_settings_lifecycle),
      _app_modal_lifecycle(app_modal_lifecycle),
      _project_opener(opener),
      _app_settings_opener(app_settings_opener) {
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
    if (auto const opener = this->_app_settings_opener.lock()) {
        return opener->can_open();
    } else {
        return false;
    }
}

void app_presenter::open_app_settings() {
    if (auto const opener = this->_app_settings_opener.lock()) {
        return opener->open();
    }
}

observing::syncable app_presenter::observe_window(std::function<void(app_presenter_window_event const &)> &&handler) {
    observing::syncable result{};

    if (auto const lifecycle = this->_project_lifecycle.lock()) {
        auto syncable = lifecycle->observe_event([handler](project_lifecycle_event const &event) {
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

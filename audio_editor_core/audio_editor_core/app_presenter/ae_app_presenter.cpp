//
//  ae_app_presenter.cpp
//

#include "ae_app_presenter.h"

#include <audio_editor_core/ae_app_global.h>
#include <audio_editor_core/ae_app_presenter_utils.h>

using namespace yas;
using namespace yas::ae;

app_presenter::app_presenter() : app_presenter(app_global()) {
}

app_presenter::app_presenter(app_ptr const &app) : _app(app) {
}

bool app_presenter::can_open_file_dialog() const {
    return true;
}

void app_presenter::open_file_dialog() {
    if (this->can_open_file_dialog()) {
    }
}

void app_presenter::add_project(url const &file_url) {
    if (auto const app = this->_app.lock()) {
        app->project_pool()->add_project(file_url);
    }
}

observing::syncable app_presenter::observe_event(std::function<void(event const &)> &&handler) {
    if (auto const app = this->_app.lock()) {
        return app->project_pool()->observe_event([handler = std::move(handler)](project_pool_event const &pool_event) {
            handler(event{.type = to_presenter_event_type(pool_event.type), .project_id = pool_event.project_id});
        });
    } else {
        return observing::syncable{};
    }
}

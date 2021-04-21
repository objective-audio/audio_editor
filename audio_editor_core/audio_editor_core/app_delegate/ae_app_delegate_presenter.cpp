//
//  ae_app_delegate_presenter.cpp
//

#include "ae_app_delegate_presenter.h"

#include <audio_editor_core/ae_app_global.h>

using namespace yas;
using namespace yas::ae;

namespace yas::ae {
static app_delegate_presenter::event_type to_presenter_event_type(project_pool_event_type const &type) {
    switch (type) {
        case project_pool_event_type::inserted:
            return app_delegate_presenter::event_type::make_and_show_window_controller;
        case project_pool_event_type::erased:
            return app_delegate_presenter::event_type::dispose_window_controller;
    }
}
}  // namespace yas::ae

app_delegate_presenter::app_delegate_presenter() : app_delegate_presenter(app_global()) {
}

app_delegate_presenter::app_delegate_presenter(app_ptr const &app) : _app(app) {
}

void app_delegate_presenter::add_project(url const &file_url) {
    this->_app->project_pool()->add_project(file_url);
}

observing::syncable app_delegate_presenter::observe_projects(std::function<void(event const &)> &&handler) {
    return this->_app->project_pool()->observe_projects(
        [handler = std::move(handler)](project_pool_event const &pool_event) {
            handler(event{.type = to_presenter_event_type(pool_event.type), .project_id = pool_event.project_id});
        });
}

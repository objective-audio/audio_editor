//
//  ae_app_presenter.cpp
//

#include "ae_app_presenter.h"

#include <audio_editor_core/ae_app.h>
#include <audio_editor_core/ae_app_presenter_utils.h>

using namespace yas;
using namespace yas::ae;

app_presenter::app_presenter() : app_presenter(app::global()->project_pool()) {
}

app_presenter::app_presenter(std::shared_ptr<app_presenter_project_pool_interface> const &pool) : _project_pool(pool) {
}

bool app_presenter::can_open_file_dialog() const {
    return true;
}

void app_presenter::open_file_dialog() {
    if (this->can_open_file_dialog()) {
        this->_event_notifier->notify(event{.type = event_type::open_file_dialog});
    }
}

void app_presenter::select_file(url const &file_url) {
    if (auto const pool = this->_project_pool.lock()) {
        pool->add_project(file_url);
    }
}

observing::syncable app_presenter::observe_event(std::function<void(event const &)> &&handler) {
    if (auto const pool = this->_project_pool.lock()) {
        auto syncable = pool->observe_event([handler](project_pool_event const &pool_event) {
            handler(event{.type = to_presenter_event_type(pool_event.type), .project_id = pool_event.project_id});
        });

        syncable.merge(this->_event_notifier->observe([handler](auto const &event) { handler(event); }));

        return syncable;
    } else {
        return observing::syncable{};
    }
}

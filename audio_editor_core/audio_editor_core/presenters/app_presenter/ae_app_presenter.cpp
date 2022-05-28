//
//  ae_app_presenter.cpp
//

#include "ae_app_presenter.h"

#include <audio_editor_core/ae_app_dialog_level.h>
#include <audio_editor_core/ae_app_dialog_sub_level_router.h>
#include <audio_editor_core/ae_app_level.h>
#include <audio_editor_core/ae_app_presenter_utils.h>
#include <audio_editor_core/ae_hierarchy.h>
#include <audio_editor_core/ae_project_level_router.h>
#include <audio_editor_core/ae_project_preparer.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<app_presenter> app_presenter::make_shared() {
    return std::make_shared<app_presenter>(hierarchy::app_level()->project_level_router,
                                           hierarchy::app_level()->dialog_level_router,
                                           hierarchy::app_level()->project_preparer);
}

app_presenter::app_presenter(std::shared_ptr<project_level_router_for_app_presenter> const &pool,
                             std::shared_ptr<app_dialog_sub_level_router> const &dialog_level_router,
                             std::shared_ptr<project_preparer> const &preparer)
    : _project_level_router(pool), _dialog_level_router(dialog_level_router), _project_preparer(preparer) {
}

bool app_presenter::can_open_dialog() const {
    if (auto const router = this->_dialog_level_router.lock()) {
        return router->dialog_level() == nullptr;
    } else {
        return false;
    }
}

void app_presenter::open_dialog() {
    if (auto const router = this->_dialog_level_router.lock()) {
        router->add_dialog();
    }
}

void app_presenter::open_project_setup_dialog() {
    if (auto const router = this->_dialog_level_router.lock()) {
        router->add_project_format_dialog();
    }
}

void app_presenter::did_close_dialog() {
    if (auto const router = this->_dialog_level_router.lock()) {
        router->remove_dialog();
    }
}

void app_presenter::select_audio_file(url const &file_url) {
    if (auto const preparer = this->_project_preparer.lock()) {
        preparer->prepare(file_url);
    }
}

void app_presenter::select_project_directory(project_format const &format, url const &file_url) {
    if (auto const preparer = this->_project_preparer.lock()) {
        preparer->prepare(format, file_url);
    }
}

observing::syncable app_presenter::observe_window(std::function<void(app_presenter_window_event const &)> &&handler) {
    if (auto const router = this->_project_level_router.lock()) {
        return router->observe_event([handler = std::move(handler)](project_level_router_event const &event) {
            handler(app_presenter_window_event{.type = to_presenter_event_type(event.type),
                                               .project_id = event.project_id});
        });
    } else {
        return observing::syncable{};
    }
}

observing::syncable app_presenter::observe_dialog(
    std::function<void(std::optional<app_dialog_sub_level> const &)> &&handler) {
    if (auto const router = this->_dialog_level_router.lock()) {
        return router->observe(std::move(handler));
    } else {
        return observing::syncable{};
    }
}

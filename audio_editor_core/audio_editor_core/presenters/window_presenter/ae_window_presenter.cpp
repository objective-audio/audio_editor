//
//  ae_window_presenter.cpp
//

#include "ae_window_presenter.h"

#include <audio_editor_core/ae_hierarchy.h>
#include <audio_editor_core/ae_project_closer.h>
#include <audio_editor_core/ae_project_url.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<window_presenter> window_presenter::make_shared(ae::project_id const &project_id) {
    auto const &project_lifetime = hierarchy::project_lifetime_for_id(project_id);
    return std::make_shared<window_presenter>(project_id, project_lifetime->project_url, project_lifetime->closer);
}

window_presenter::window_presenter(ae::project_id const &project_id, std::shared_ptr<project_url> const &project_url,
                                   std::shared_ptr<project_closer_for_window_presenter> const &closer)
    : project_id(project_id), _project_url(project_url), _closer(closer) {
}

std::string window_presenter::title() const {
    if (auto const project_url = this->_project_url.lock()) {
        return project_url->root_directory().last_path_component();
    } else {
        return "";
    }
}

bool window_presenter::should_close() {
    auto const &closer = this->_closer.lock();

    if (closer && closer->can_close()) {
        closer->request_close();
        return true;
    } else {
        return false;
    }
}

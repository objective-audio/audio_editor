//
//  ae_window_presenter.cpp
//

#include "ae_window_presenter.h"

#include <audio_editor_core/ae_hierarchy.h>
#include <audio_editor_core/ae_project_closer.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<window_presenter> window_presenter::make_shared(ae::project_id const &project_id) {
    auto const &project_level = hierarchy::project_level_for_id(project_id);
    return std::make_shared<window_presenter>(project_id, project_level->file_url, project_level->closer);
}

window_presenter::window_presenter(ae::project_id const &project_id, url const &file_url,
                                   std::shared_ptr<project_closer_for_window_presenter> const &closer)
    : project_id(project_id), _file_url(file_url), _closer(closer) {
}

std::string window_presenter::title() const {
    return this->_file_url.last_path_component();
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

//
//  ae_module_name_presenter.cpp
//

#include "ae_module_name_presenter.h"

#include <audio_editor_core/ae_file_track.h>
#include <audio_editor_core/ae_hierarchy.h>
#include <audio_editor_core/ae_project_sub_level_router.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<module_name_presenter> module_name_presenter::make_shared(project_id const &project_id,
                                                                          time::range const module_range) {
    auto const &project_level = hierarchy::project_level_for_id(project_id);
    return std::make_shared<module_name_presenter>(module_range, project_level->file_track,
                                                   project_level->sub_level_router);
}

module_name_presenter::module_name_presenter(time::range const &module_range,
                                             std::shared_ptr<file_track> const &file_track,
                                             std::shared_ptr<project_sub_level_router> const &router)
    : _module_range(module_range), _file_track(file_track), _router(router) {
}

std::string const &module_name_presenter::name() const {
    if (auto const file_track = this->_file_track.lock()) {
        if (file_track->modules().contains(this->_module_range)) {
            return file_track->modules().at(this->_module_range).name;
        }
    }

    static std::string const empty_name = "";
    return empty_name;
}

void module_name_presenter::done(std::string const &name) {
    if (auto const file_track = this->_file_track.lock()) {
        file_track->set_module_name_and_notify(this->_module_range, name);
    }

    this->_finalize();
}

void module_name_presenter::cancel() {
    this->_finalize();
}

void module_name_presenter::_finalize() {
    if (auto const router = this->_router.lock()) {
        router->remove_sheet();
    }

    this->_file_track.reset();
    this->_router.reset();
}

//
//  ae_root_presenter.cpp
//

#include "ae_root_presenter.h"

#include <audio_editor_core/ae_hierarchy.h>
#include <audio_editor_core/ae_project_editor_level_pool.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<root_presenter> root_presenter::make_shared(std::string const &project_id) {
    auto const &project_level = hierarchy::project_level_for_id(project_id);
    return std::shared_ptr<root_presenter>(new root_presenter{project_level->editor_level_pool});
}

root_presenter::root_presenter(std::shared_ptr<project_editor_level_pool> const &editor_level_pool)
    : _editor_level_pool(editor_level_pool) {
}

observing::syncable root_presenter::observe_is_editing(std::function<void(bool const &)> &&handler) {
    if (auto const pool = this->_editor_level_pool.lock()) {
        return pool->observe_level([handler = std::move(handler)](const std::shared_ptr<project_editor_level> &level) {
            handler(level != nullptr);
        });
    } else {
        return observing::syncable{};
    }
}

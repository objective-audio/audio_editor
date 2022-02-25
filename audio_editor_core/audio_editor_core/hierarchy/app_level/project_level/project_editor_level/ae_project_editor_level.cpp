//
//  ae_project_editor_level.cpp
//

#include "ae_project_editor_level.h"

using namespace yas;
using namespace yas::ae;

std::shared_ptr<project_editor_level> project_editor_level::make_shared(std::shared_ptr<project_editor> const &editor) {
    return std::shared_ptr<project_editor_level>(new project_editor_level{editor});
}

project_editor_level::project_editor_level(std::shared_ptr<project_editor> const &editor) : editor(editor) {
}

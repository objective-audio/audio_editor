//
//  ae_ui_editing_root_maker.mm
//

#include "ae_ui_editing_root_maker.h"
#include <audio_editor_core/ae_ui_editing_root.h>

using namespace yas;
using namespace yas::ae;

ui_editing_root_maker::ui_editing_root_maker(std::string const &project_id, std::uintptr_t const project_view_id)
    : _project_id(project_id), _project_view_id(project_view_id) {
}

std::shared_ptr<ui_editing_root_maker> ui_editing_root_maker::make_shared(std::string const &project_id,
                                                                          std::uintptr_t const project_view_id) {
    return std::shared_ptr<ui_editing_root_maker>(new ui_editing_root_maker{project_id, project_view_id});
}

std::shared_ptr<ui_editing_root> ui_editing_root_maker::make() const {
    return ui_editing_root::make_shared(this->_project_id, this->_project_view_id);
}

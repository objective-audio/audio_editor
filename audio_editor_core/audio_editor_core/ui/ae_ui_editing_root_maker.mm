//
//  ae_ui_editing_root_maker.mm
//

#include "ae_ui_editing_root_maker.h"
#include <audio_editor_core/ae_ui_editing_root.h>

using namespace yas;
using namespace yas::ae;

ui_editing_root_maker::ui_editing_root_maker(std::shared_ptr<ui::standard> const &standard,
                                             std::string const &project_id)
    : _standard(standard), _project_id(project_id) {
}

std::shared_ptr<ui_editing_root_maker> ui_editing_root_maker::make_shared(std::shared_ptr<ui::standard> const &standard,
                                                                          std::string const &project_id) {
    return std::shared_ptr<ui_editing_root_maker>(new ui_editing_root_maker{standard, project_id});
}

std::shared_ptr<ui_editing_root> ui_editing_root_maker::make() const {
    return ui_editing_root::make_shared(this->_standard, this->_project_id);
}

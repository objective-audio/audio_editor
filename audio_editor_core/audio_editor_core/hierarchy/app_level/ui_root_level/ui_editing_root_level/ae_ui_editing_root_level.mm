//
//  ae_ui_editing_root_level.mm
//

#include "ae_ui_editing_root_level.h"
#include <audio_editor_core/ae_ui_editing_root.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<ui_editing_root_level> ui_editing_root_level::make_shared(ui_project_id const &project_id) {
    return std::shared_ptr<ui_editing_root_level>(new ui_editing_root_level{project_id});
}

ui_editing_root_level::ui_editing_root_level(ui_project_id const &project_id)
    : editing_root(ui_editing_root::make_shared(project_id)) {
}

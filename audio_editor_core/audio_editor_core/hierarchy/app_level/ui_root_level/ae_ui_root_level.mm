//
//  ae_ui_root_level.mm
//

#include "ae_ui_root_level.h"
#include <audio_editor_core/ae_ui_root.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<ui_root_level> ui_root_level::make_shared(std::shared_ptr<ui::standard> const &standard,
                                                          std::string const &project_id,
                                                          std::uintptr_t const project_view_id) {
    return std::shared_ptr<ui_root_level>(new ui_root_level{standard, project_id, project_view_id});
}

ui_root_level::ui_root_level(std::shared_ptr<ui::standard> const &standard, std::string const &project_id,
                             std::uintptr_t const project_view_id)
    : ui_root(ui_root::make_shared(standard, project_id, project_view_id)) {
}

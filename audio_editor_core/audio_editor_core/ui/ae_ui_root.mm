//
//  ae_ui_root.mm
//

#include "ae_ui_root.h"
#include <audio_editor_core/ae_display_space.h>
#include <audio_editor_core/ae_pinch_gesture_controller.h>
#include <audio_editor_core/ae_root_presenter.h>
#include <audio_editor_core/ae_ui_editing_root.h>
#include <audio_editor_core/ae_ui_editing_root_maker.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<ui_root> ui_root::make_shared(std::shared_ptr<ui::standard> const &standard,
                                              std::string const &project_id, std::uintptr_t const project_view_id) {
    auto const presenter = root_presenter::make_shared(project_id);
    auto const editing_root_maker = ui_editing_root_maker::make_shared(project_id, project_view_id);
    return std::shared_ptr<ui_root>(new ui_root{project_view_id, standard, presenter, editing_root_maker});
}

ui_root::ui_root(std::uintptr_t const project_view_id, std::shared_ptr<ui::standard> const &standard,
                 std::shared_ptr<root_presenter> const &presenter,
                 std::shared_ptr<ui_editing_root_maker_for_ui_root> const &editing_root_maker)
    : _project_view_id(project_view_id), _presenter(presenter), _editing_root_maker(editing_root_maker) {
    presenter
        ->observe_is_editing([this](bool const &is_editing) {
            if (is_editing && !this->_editing_root) {
                this->_editing_root = this->_editing_root_maker->make();
            }
        })
        .sync()
        ->add_to(this->_pool);
}

bool ui_root::responds_to_action(action const action) {
    if (auto const &editing_root = this->_editing_root) {
        return editing_root->responds_to_action(action);
    } else {
        return false;
    }
}

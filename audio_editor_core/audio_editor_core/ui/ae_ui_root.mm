//
//  ae_ui_root.mm
//

#include "ae_ui_root.h"
#include <audio_editor_core/ae_action_controller.h>
#include <audio_editor_core/ae_pinch_gesture_controller.h>
#include <audio_editor_core/ae_root_presenter.h>
#include <audio_editor_core/ae_ui_editing_root.h>
#include <audio_editor_core/ae_ui_editing_root_maker.h>

using namespace yas;
using namespace yas::ae;

ui_root::ui_root(std::shared_ptr<ui::standard> const &standard, std::shared_ptr<root_presenter> const &presenter,
                 std::shared_ptr<action_controller> const &action_controller,
                 std::shared_ptr<pinch_gesture_controller> const &pinch_gesture_controller,
                 std::shared_ptr<ui_editing_root_maker_for_ui_root> const &editing_root_maker)
    : _standard(standard),
      _presenter(presenter),
      _action_controller(action_controller),
      _pinch_gesture_controller(pinch_gesture_controller),
      _editing_root_maker(editing_root_maker) {
    presenter
        ->observe_is_editing([this](bool const &is_editing) {
            if (is_editing && !this->_editing_root) {
                this->_editing_root = this->_editing_root_maker->make();
            }
        })
        .sync()
        ->add_to(this->_pool);
}

std::shared_ptr<ui_root> ui_root::make_shared(std::shared_ptr<ui::standard> const &standard,
                                              std::string const &project_id) {
    auto const presenter = root_presenter::make_shared(project_id);
    auto const controller = action_controller::make_shared(project_id);
    auto const pinch_gesture_controller = pinch_gesture_controller::make_shared(project_id);
    auto const editing_root_maker = ui_editing_root_maker::make_shared(standard, project_id);
    return std::shared_ptr<ui_root>(
        new ui_root{standard, presenter, controller, pinch_gesture_controller, editing_root_maker});
}

std::shared_ptr<ui::standard> const &ui_root::standard() const {
    return this->_standard;
}

bool ui_root::responds_to_action(action const action) {
    return this->_presenter->responds_to_action(action);
}

void ui_root::handle_action(action const action) {
    this->_action_controller->handle_action(action);
}

void ui_root::handle_pinch_gesture(pinch_gesture const &gesture) {
    this->_pinch_gesture_controller->handle_gesture(gesture);
}

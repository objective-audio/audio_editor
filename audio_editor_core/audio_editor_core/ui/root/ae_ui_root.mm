//
//  ae_ui_root.mm
//

#include "ae_ui_root.h"
#include <audio_editor_core/ae_app_lifetime.h>
#include <audio_editor_core/ae_color.h>
#include <audio_editor_core/ae_display_space.h>
#include <audio_editor_core/ae_hierarchy.h>
#include <audio_editor_core/ae_root_presenter.h>
#include <audio_editor_core/ae_ui_editing_root.h>

#include <cpp_utils/yas_assertion.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<ui_root> ui_root::make_shared(std::shared_ptr<ui::standard> const &standard,
                                              ui_project_id const &ui_project_id,
                                              std::shared_ptr<ui_editing_root> const &editing_root) {
    auto const &app_lifetime = hierarchy::app_lifetime();
    auto const presenter = root_presenter::make_shared(ui_project_id.window_lifetime_id.project_id);
    return std::shared_ptr<ui_root>(new ui_root{app_lifetime->color, standard, presenter, editing_root});
}

ui_root::ui_root(std::shared_ptr<ae::color> const &color, std::shared_ptr<ui::standard> const &standard,
                 std::shared_ptr<root_presenter> const &presenter, std::shared_ptr<ui_editing_root> const &editing_root)
    : _presenter(presenter),
      _root_node(standard->root_node()),
      _color(color),
      _background(standard->view_look()->background()),
      _editing_root(editing_root) {
    standard->view_look()
        ->observe_appearance([this](auto const &) {
            auto const background = this->_background.lock();
            auto const color = this->_color.lock();

            if (background && color) {
                background->set_color(color->background());
            }
        })
        .sync()
        ->add_to(this->_pool);

    this->_root_node->add_sub_node(editing_root->node);
}

bool ui_root::responds_to_action(action const action) {
    auto const editing_root = this->_editing_root.lock();
    if (!editing_root) {
        assertion_failure_if_not_test();
        return false;
    }

    return editing_root->responds_to_action(action);
}

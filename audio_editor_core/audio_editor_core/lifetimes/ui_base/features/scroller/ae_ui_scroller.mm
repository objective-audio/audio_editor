//
//  ae_ui_scroller.mm
//

#include "ae_ui_scroller.h"
#include <audio_editor_core/ae_hierarchy.h>
#include <audio_editor_core/ae_scroll_gesture_controller.h>
#include <audio_editor_core/ae_scroller_presenter.h>
#include <audio_editor_core/ae_ui_edge.h>
#include <audio_editor_core/ae_ui_hierarchy.h>
#include <audio_editor_core/ae_ui_markers.h>
#include <audio_editor_core/ae_ui_zooming.h>
#include <audio_editor_core/ae_ui_zooming_constants.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<ui_scroller> ui_scroller::make_shared(project_lifetime_id const &project_lifetime_id,
                                                      ui_scroller_nodes const &scroller_nodes) {
    auto const presenter = scroller_presenter::make_shared(project_lifetime_id);
    auto const &project_editing_lifetime = hierarchy::project_editing_lifetime_for_id(project_lifetime_id);
    auto const &resource_lifetime = ui_hierarchy::resource_lifetime_for_project_lifetime_id(project_lifetime_id);

    return std::make_shared<ui_scroller>(resource_lifetime->standard, scroller_nodes, presenter,
                                         project_editing_lifetime->scroll_gesture_controller);
}

ui_scroller::ui_scroller(std::shared_ptr<ui::standard> const &standard, ui_scroller_nodes const &scroller_nodes,
                         std::shared_ptr<scroller_presenter> const &presenter,
                         std::shared_ptr<scroll_gesture_controller> const &scroll_gesture_controller)
    : _presenter(presenter),
      _scroll_gesture_controller(scroll_gesture_controller),
      _back_x_fixed_node(scroller_nodes.back_x_fixed.get()),
      _back_y_fixed_node(scroller_nodes.back_y_fixed.get()),
      _modules_node(scroller_nodes.modules.get()),
      _front_y_fixed_node(scroller_nodes.front_y_fixed.get()),
      _front_x_fixed_node(scroller_nodes.front_x_fixed.get()) {
    standard->renderer()
        ->observe_will_render([this](auto const &) {
            float const x = this->_presenter->x();
            float const y = this->_presenter->y();
            this->_modules_node->set_position(ui::point{x, y});

            ui::point const y_fixed_position{x, 0.0f};
            this->_back_y_fixed_node->set_position(y_fixed_position);
            this->_front_y_fixed_node->set_position(y_fixed_position);

            ui::point const x_fixed_position{0.0f, y};
            this->_back_x_fixed_node->set_position(y_fixed_position);
            this->_front_x_fixed_node->set_position(x_fixed_position);
        })
        .end()
        ->add_to(this->_pool);

    standard->event_manager()
        ->observe([this](std::shared_ptr<ui::event> const &event) {
            if (event->type() == ui::event_type::scroll) {
                auto const &scroll_event = event->get<ui::scroll>();
                gesture_state const state = to_gesture_state(event->phase());

                this->_scroll_gesture_controller->handle_gesture(
                    scroll_gesture{.state = state, .delta_x = scroll_event.delta_x, .delta_y = scroll_event.delta_y});
            }
        })
        .end()
        ->add_to(this->_pool);
}

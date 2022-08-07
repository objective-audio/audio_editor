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
#include <audio_editor_core/ae_ui_track.h>
#include <audio_editor_core/ae_ui_track_constants.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<ui_scroller> ui_scroller::make_shared(window_lifetime_id const &window_lifetime_id, ui::node *node) {
    auto const presenter = scroller_presenter::make_shared(window_lifetime_id);
    auto const &project_lifetime = hierarchy::project_lifetime_for_id(window_lifetime_id);
    auto const &resource_lifetime = ui_hierarchy::resource_lifetime_for_window_lifetime_id(window_lifetime_id);

    return std::make_shared<ui_scroller>(resource_lifetime->standard, node, presenter,
                                         project_lifetime->scroll_gesture_controller);
}

ui_scroller::ui_scroller(std::shared_ptr<ui::standard> const &standard, ui::node *node,
                         std::shared_ptr<scroller_presenter> const &presenter,
                         std::shared_ptr<scroll_gesture_controller> const &scroll_gesture_controller)
    : _presenter(presenter), _scroll_gesture_controller(scroll_gesture_controller), _node(node) {
    standard->renderer()
        ->observe_will_render([this](auto const &) {
            auto const time = this->_presenter->current_position();
            auto const scale = this->_presenter->horizontal_zooming_scale();
            float const x = -time * ui_track_constants::standard_width_per_sec * scale;
            this->_node->set_position(ui::point{x, 0.0f});
        })
        .end()
        ->add_to(this->_pool);

    standard->event_manager()
        ->observe([this](std::shared_ptr<ui::event> const &event) {
            if (event->type() == ui::event_type::scroll) {
                auto const &scroll_event = event->get<ui::scroll>();
                gesture_state const state = to_gesture_state(event->phase());

                auto const sec_width =
                    ui_track_constants::standard_width_per_sec * this->_presenter->horizontal_zooming_scale();
                auto const delta_time = -scroll_event.delta_x / sec_width;

                this->_scroll_gesture_controller->handle_gesture(
                    scroll_gesture{.state = state, .delta_time = delta_time});
            }
        })
        .end()
        ->add_to(this->_pool);
}

//
//  ae_ui_scroller.mm
//

#include "ae_ui_scroller.h"
#include <audio_editor_core/ae_scroll_gesture_controller.h>
#include <audio_editor_core/ae_scroller_presenter.h>
#include <audio_editor_core/ae_ui_edge.h>
#include <audio_editor_core/ae_ui_hierarchy.h>
#include <audio_editor_core/ae_ui_markers.h>
#include <audio_editor_core/ae_ui_track.h>
#include <audio_editor_core/ae_ui_track_constants.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<ui_scroller> ui_scroller::make_shared(ui_project_id const &project_id,
                                                      std::shared_ptr<ui::standard> const &standard,
                                                      std::shared_ptr<ui_track> const &track,
                                                      std::shared_ptr<ui_edge> const &edge,
                                                      std::shared_ptr<ui_markers> const &markers) {
    auto const presenter = scroller_presenter::make_shared(project_id.project_id);
    auto const scroll_gestore_controller = scroll_gesture_controller::make_shared(project_id.project_id);
    return std::make_shared<ui_scroller>(standard, presenter, scroll_gestore_controller, track, edge, markers);
}

ui_scroller::ui_scroller(std::shared_ptr<ui::standard> const &standard,
                         std::shared_ptr<scroller_presenter> const &presenter,
                         std::shared_ptr<scroll_gesture_controller> const &scroll_gesture_controller,
                         std::shared_ptr<ui_track> const &track, std::shared_ptr<ui_edge> const &edge,
                         std::shared_ptr<ui_markers> const &markers)
    : node(ui::node::make_shared()),
      _presenter(presenter),
      _scroll_gesture_controller(scroll_gesture_controller),
      _scrolling_node(ui::node::make_shared()),
      _track(track),
      _edge(edge),
      _markers(markers) {
    this->node->add_sub_node(this->_scrolling_node);
    this->_scrolling_node->add_sub_node(this->_track->node);
    this->_scrolling_node->add_sub_node(this->_edge->node);
    this->_scrolling_node->add_sub_node(this->_markers->node);

    standard->renderer()
        ->observe_will_render([this](auto const &) {
            auto const time = this->_presenter->current_position();
            auto const scale = this->_presenter->horizontal_zooming_scale();
            float const x = -time * ui_track_constants::standard_width_per_sec * scale;
            this->_scrolling_node->set_position(ui::point{x, 0.0f});
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

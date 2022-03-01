//
//  ae_ui_track.mm
//

#include "ae_ui_track.h"
#include <audio_editor_core/ae_display_space.h>
#include <audio_editor_core/ae_gesture.h>
#include <audio_editor_core/ae_scroll_gesture_controller.h>
#include <audio_editor_core/ae_track_presenter.h>
#include <audio_editor_core/ae_ui_edge.h>
#include <audio_editor_core/ae_ui_hierarchy.h>
#include <audio_editor_core/ae_ui_markers.h>
#include <audio_editor_core/ae_ui_modules.h>
#include <audio_editor_core/ae_ui_root_level.h>
#include <audio_editor_core/ae_ui_track_constants.h>
#include <cpp_utils/yas_fast_each.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<ui_track> ui_track::make_shared(std::string const &project_id, uintptr_t const project_view_id) {
    auto const &ui_root_level = ui_hierarchy::root_level_for_view_id(project_view_id);

    auto const presenter = track_presenter::make_shared(project_id);
    auto const scroll_gestore_controller = scroll_gesture_controller::make_shared(project_id);
    auto const modules = ui_modules::make_shared(project_id, project_view_id);
    auto const edge = ui_edge::make_shared(project_id, project_view_id);
    auto const markers = ui_markers::make_shared(project_id, project_view_id);
    return std::shared_ptr<ui_track>(new ui_track{ui_root_level->standard, ui_root_level->display_space, presenter,
                                                  scroll_gestore_controller, modules, edge, markers});
}

ui_track::ui_track(std::shared_ptr<ui::standard> const &standard, std::shared_ptr<display_space> const &display_space,
                   std::shared_ptr<track_presenter> const &presenter,
                   std::shared_ptr<scroll_gesture_controller> const &scroll_gesture_controller,
                   std::shared_ptr<ui_modules> const &modules, std::shared_ptr<ui_edge> const &edge,
                   std::shared_ptr<ui_markers> const &markers)
    : _standard(standard),
      _display_space(display_space),
      _presenter(presenter),
      _scroll_gesture_controller(scroll_gesture_controller),
      _root_node(ui::node::make_shared()),
      _time_node(ui::node::make_shared()),
      _modules(modules),
      _edge(edge),
      _markers(markers) {
    this->_root_node->add_sub_node(this->_time_node);
    this->_time_node->add_sub_node(this->_modules->node());
    this->_time_node->add_sub_node(this->_edge->node());
    this->_time_node->add_sub_node(this->_markers->node());

    standard->renderer()
        ->observe_will_render([this](auto const &) {
            auto const time = this->_presenter->current_position();
            auto const scale = this->_presenter->horizontal_zooming_scale();
            float const x = -time * ui_track_constants::standard_width_per_sec * scale;
            this->_time_node->set_position(ui::point{x, 0.0f});
        })
        .end()
        ->add_to(this->_pool);

    presenter->observe_horizontal_zooming_scale([this](double const &) { this->_update_scale(); })
        .sync()
        ->add_to(this->_pool);
    presenter->observe_vertical_zooming_scale([this](double const &) { this->_update_scale(); })
        .sync()
        ->add_to(this->_pool);

    this->_standard->event_manager()
        ->observe([this](std::shared_ptr<ui::event> const &event) {
            if (event->type() == ui::event_type::scroll) {
                auto const &scroll_event = event->get<ui::scroll>();
                gesture_state const state = to_gesture_state(event->phase());

                auto const sec_width =
                    ui_track_constants::standard_width_per_sec * this->_presenter->horizontal_zooming_scale();
                auto const delta_time = -scroll_event.deltaX() / sec_width;

                this->_scroll_gesture_controller->handle_gesture(
                    scroll_gesture{.state = state, .delta_time = delta_time});
            }
        })
        .end()
        ->add_to(this->_pool);

    standard->view_look()
        ->view_layout_guide()
        ->observe([this](ui::region const &region) {
            this->_display_space->set_view_region(region);
            this->_update_scale();
        })
        .sync()
        ->add_to(this->_pool);
}

std::shared_ptr<ui::node> const ui_track::node() const {
    return this->_root_node;
}

void ui_track::_update_scale() {
    float const width = this->_presenter->horizontal_zooming_scale() * ui_track_constants::standard_width_per_sec;
    float const height = std::ceil(this->_presenter->vertical_zooming_scale() * ui_track_constants::standard_height);

    ui::size const scale{width, height};
    this->_modules->set_scale(scale);
    this->_display_space->set_scale(scale);
}

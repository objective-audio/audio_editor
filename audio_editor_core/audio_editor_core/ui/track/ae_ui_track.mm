//
//  ae_ui_track.mm
//

#include "ae_ui_track.h"
#include <audio_editor_core/ae_display_space.h>
#include <audio_editor_core/ae_gesture.h>
#include <audio_editor_core/ae_scroll_gesture_controller.h>
#include <audio_editor_core/ae_track_presenter.h>
#include <audio_editor_core/ae_ui_markers.h>
#include <audio_editor_core/ae_ui_modules.h>
#include <audio_editor_core/ae_ui_track_constants.h>
#include <cpp_utils/yas_fast_each.h>

using namespace yas;
using namespace yas::ae;

ui_track::ui_track(std::shared_ptr<ui::standard> const &standard, std::shared_ptr<track_presenter> const &presenter,
                   std::shared_ptr<scroll_gesture_controller> const &scroll_gesture_controller,
                   std::shared_ptr<ui_modules> const &modules, std::shared_ptr<ui_markers> const &markers)
    : _standard(standard),
      _display_space(display_space::make_shared(standard->view_look()->view_layout_guide()->region())),
      _presenter(presenter),
      _scroll_gesture_controller(scroll_gesture_controller),
      _root_node(ui::node::make_shared()),
      _time_node(ui::node::make_shared()),
      _modules(modules),
      _markers(markers) {
    this->_root_node->add_sub_node(this->_time_node);
    this->_time_node->add_sub_node(this->_modules->node());
    this->_time_node->add_sub_node(this->_markers->node());

    standard->renderer()
        ->observe_will_render([this](auto const &) {
            auto const time = this->_presenter->current_position();
            auto const scale = this->_presenter->scale();
            float const x = -time * ui_track_constants::width_per_sec * scale;
            this->_time_node->set_position(ui::point{x, 0.0f});
        })
        .end()
        ->add_to(this->_pool);

    presenter
        ->observe_scale([this](double const &value) {
            ui::size const scale{static_cast<float>(value * ui_track_constants::width_per_sec),
                                 ui_track_constants::height};
            this->_modules->set_scale(scale);
            this->_markers->set_scale(scale);
            this->_display_space->set_scale(scale);
        })
        .sync()
        ->add_to(this->_pool);

    this->_standard->event_manager()
        ->observe([this](std::shared_ptr<ui::event> const &event) {
            if (event->type() == ui::event_type::scroll) {
                auto const &scroll_event = event->get<ui::scroll>();
                gesture_state const state = to_gesture_state(event->phase());

                auto const sec_width = ui_track_constants::width_per_sec * this->_presenter->scale();
                auto const delta_time = -scroll_event.deltaX() / sec_width;

                this->_scroll_gesture_controller->handle_gesture(
                    scroll_gesture{.state = state, .delta_time = delta_time});
            }
        })
        .end()
        ->add_to(this->_pool);
}

std::shared_ptr<ui::node> const ui_track::node() const {
    return this->_root_node;
}

std::shared_ptr<ui_track> ui_track::make_shared(std::shared_ptr<ui::standard> const &standard,
                                                std::string const &project_id) {
    auto const presenter = track_presenter::make_shared(project_id);
    auto const scroll_gestore_controller = scroll_gesture_controller::make_shared(project_id);
    auto const modules = ui_modules::make_shared(project_id);
    auto const markers = ui_markers::make_shared(project_id);
    return std::shared_ptr<ui_track>(new ui_track{standard, presenter, scroll_gestore_controller, modules, markers});
}

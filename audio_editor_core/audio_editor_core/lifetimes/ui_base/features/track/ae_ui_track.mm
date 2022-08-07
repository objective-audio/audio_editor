//
//  ae_ui_track.mm
//

#include "ae_ui_track.h"
#include <audio_editor_core/ae_display_space.h>
#include <audio_editor_core/ae_track_presenter.h>
#include <audio_editor_core/ae_ui_hierarchy.h>
#include <audio_editor_core/ae_ui_modules.h>
#include <audio_editor_core/ae_ui_track_constants.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<ui_track> ui_track::make_shared(window_lifetime_id const &window_lifetime_id, ui_modules *modules) {
    auto const presenter = track_presenter::make_shared(window_lifetime_id);
    auto const &resource_lifetime = ui_hierarchy::resource_lifetime_for_window_lifetime_id(window_lifetime_id);

    return std::make_shared<ui_track>(resource_lifetime->standard, resource_lifetime->display_space.get(), presenter,
                                      modules);
}

ui_track::ui_track(std::shared_ptr<ui::standard> const &standard, display_space *display_space,
                   std::shared_ptr<track_presenter> const &presenter, ui_modules *modules)
    : _display_space(display_space), _presenter(presenter), _modules(modules) {
    presenter->observe_zooming_scale([this](auto const &) { this->_update_scale(); }).sync()->add_to(this->_pool);

    standard->view_look()
        ->view_layout_guide()
        ->observe([this](ui::region const &region) {
            this->_display_space->set_view_region(region);
            this->_update_scale();
        })
        .sync()
        ->add_to(this->_pool);
}

void ui_track::_update_scale() {
    auto const zooming_scale = this->_presenter->zooming_scale();
    float const width = zooming_scale.horizontal * ui_track_constants::standard_width_per_sec;
    float const height = std::ceil(zooming_scale.vertical * ui_track_constants::standard_height);

    ui::size const scale{width, height};
    this->_modules->set_scale(scale);
    this->_display_space->set_scale(scale);
}

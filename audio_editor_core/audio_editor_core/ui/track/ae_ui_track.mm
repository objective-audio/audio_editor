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

std::shared_ptr<ui_track> ui_track::make_shared(ui_project_id const &project_id,
                                                std::shared_ptr<ui_modules> const &modules) {
    auto const &ui_root_level = ui_hierarchy::root_level_for_view_id(project_id.view_id);
    auto const presenter = track_presenter::make_shared(project_id.identifier);
    return std::shared_ptr<ui_track>(
        new ui_track{ui_root_level->standard, ui_root_level->display_space, presenter, modules});
}

ui_track::ui_track(std::shared_ptr<ui::standard> const &standard, std::shared_ptr<display_space> const &display_space,
                   std::shared_ptr<track_presenter> const &presenter, std::shared_ptr<ui_modules> const &modules)
    : node(ui::node::make_shared()), _display_space(display_space), _presenter(presenter), _modules(modules) {
    this->node->add_sub_node(this->_modules->node);

    presenter->observe_horizontal_zooming_scale([this](double const &) { this->_update_scale(); })
        .sync()
        ->add_to(this->_pool);
    presenter->observe_vertical_zooming_scale([this](double const &) { this->_update_scale(); })
        .sync()
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

void ui_track::_update_scale() {
    float const width = this->_presenter->horizontal_zooming_scale() * ui_track_constants::standard_width_per_sec;
    float const height = std::ceil(this->_presenter->vertical_zooming_scale() * ui_track_constants::standard_height);

    ui::size const scale{width, height};
    this->_modules->set_scale(scale);
    this->_display_space->set_scale(scale);
}

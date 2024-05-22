//
//  ae_ui_zooming.mm
//

#include "ae_ui_zooming.h"
#include <ae-core/global/ae_ui_hierarchy.h>
#include <ae-core/project/features/ae_display_space.h>
#include <ae-core/ui_base/features/modules/ae_ui_modules.h>
#include <ae-core/ui_base/features/zooming/ae_ui_zooming_constants.h>
#include <ae-core/ui_base/features/zooming/presenter/ae_zooming_presenter.h>
#include <ae-core/ui_base/features/grid/ae_ui_grid.hpp>
#include <ae-core/ui_base/features/pasting_modules/ae_ui_pasting_modules.hpp>
#include <ae-core/ui_base/features/tracks/ae_ui_tracks.hpp>
#include <ae-core/ui_base/features/tracks_bg/ae_ui_tracks_bg.hpp>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<ui_zooming> ui_zooming::make_shared(project_lifetime_id const &project_lifetime_id, ui_modules *modules,
                                                    ui_pasting_modules *pasting_modules, ui_grid *grid,
                                                    ui_tracks *tracks, ui_tracks_bg *tracks_bg) {
    auto const presenter = zooming_presenter::make_shared(project_lifetime_id);
    auto const &project_lifetime = hierarchy::project_lifetime_for_id(project_lifetime_id);
    auto const &resource_lifetime = ui_hierarchy::resource_lifetime_for_project_lifetime_id(project_lifetime_id);

    return std::make_shared<ui_zooming>(resource_lifetime->standard, project_lifetime->display_space.get(), presenter,
                                        modules, pasting_modules, grid, tracks, tracks_bg);
}

ui_zooming::ui_zooming(std::shared_ptr<ui::standard> const &standard, display_space *display_space,
                       std::shared_ptr<zooming_presenter> const &presenter, ui_modules *modules,
                       ui_pasting_modules *pasting_modules, ui_grid *grid, ui_tracks *tracks, ui_tracks_bg *tracks_bg)
    : _display_space(display_space),
      _presenter(presenter),
      _modules(modules),
      _pasting_modules(pasting_modules),
      _grid(grid),
      _tracks(tracks),
      _tracks_bg(tracks_bg) {
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

void ui_zooming::_update_scale() {
    auto const zooming_size = to_zooming_size(this->_presenter->zooming_scale());

    ui::size const scale{static_cast<float>(zooming_size.width_per_sec),
                         static_cast<float>(zooming_size.height_per_track)};
    this->_display_space->set_scale(scale);
    this->_modules->set_scale(scale);
    this->_pasting_modules->set_scale(scale);
    this->_grid->set_scale(scale);
    this->_tracks->set_scale(scale);
    this->_tracks_bg->set_scale(scale);
}

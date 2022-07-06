//
//  ae_edge_presenter.cpp
//

#include "ae_edge_presenter.h"

#include <audio_editor_core/ae_display_space.h>
#include <audio_editor_core/ae_edge_holder.h>
#include <audio_editor_core/ae_file_info.h>
#include <audio_editor_core/ae_hierarchy.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<edge_presenter> edge_presenter::make_shared(window_lifetime_id const &window_lifetime_id,
                                                            std::shared_ptr<display_space> const &display_space) {
    auto const &project_lifetime = hierarchy::project_lifetime_for_id(window_lifetime_id);
    return std::shared_ptr<edge_presenter>(
        new edge_presenter{project_lifetime->project_format, project_lifetime->edge_holder, display_space});
}

edge_presenter::edge_presenter(project_format const &project_format, std::shared_ptr<edge_holder> const &edge_holder,
                               std::shared_ptr<display_space> const &display_space)
    : _project_format(project_format),
      _locations(observing::value::holder<edge_locations>::make_shared({.begin = {.x = 0}, .end = {.x = 0}})),
      _edge_holder(edge_holder),
      _display_space(display_space) {
    edge_holder->observe_event([this](edge_holder_event const &) { this->_update_locations(); })
        .end()
        ->add_to(this->_pool);

    display_space->observe([this](display_space_event const &) { this->_update_locations(); })
        .sync()
        ->add_to(this->_pool);
}

edge_locations edge_presenter::locations() const {
    return this->_locations->value();
}

observing::syncable edge_presenter::observe_locations(std::function<void(edge_locations const &)> &&handler) {
    return this->_locations->observe(std::move(handler));
}

void edge_presenter::_update_locations() {
    auto const edge_holder = this->_edge_holder.lock();
    auto const display_space = this->_display_space.lock();

    if (edge_holder && display_space) {
        auto const &edge = edge_holder->edge();
        auto const sample_rate = this->_project_format.sample_rate;
        auto const &scale = display_space->scale();

        this->_locations->set_value({.begin = edge_location::make_value(edge.begin_frame, sample_rate, scale),
                                     .end = edge_location::make_value(edge.end_frame, sample_rate, scale)});
    } else {
        this->_locations->set_value({.begin = {.x = 0}, .end = {.x = 0}});
    }
}

//
//  edge_presenter.cpp
//

#include "edge_presenter.h"

#include <ae-core/app/value_types/file_info.h>
#include <ae-core/global/hierarchy.h>
#include <ae-core/project/features/display_space.h>
#include <ae-core/project_editing/features/edge_holder.h>
#include <cpp-utils/lock.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<edge_presenter> edge_presenter::make_shared(project_lifetime_id const &project_lifetime_id) {
    auto const &project_lifetime = hierarchy::project_lifetime_for_id(project_lifetime_id);
    auto const &project_editing_lifetime = hierarchy::project_editing_lifetime_for_id(project_lifetime_id);
    return std::shared_ptr<edge_presenter>(new edge_presenter{project_editing_lifetime->project_format,
                                                              project_editing_lifetime->edge_holder,
                                                              project_lifetime->display_space});
}

edge_presenter::edge_presenter(project_format const &project_format, std::shared_ptr<edge_holder> const &edge_holder,
                               std::shared_ptr<display_space> const &display_space)
    : _project_format(project_format),
      _locations(observing::value::holder<edge_locations>::make_shared({.begin = {0}, .end = {0}})),
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
    auto const locked = yas::lock(this->_edge_holder, this->_display_space);

    if (fulfilled(locked)) {
        auto const &[edge_holder, display_space] = locked;
        auto const &edge = edge_holder->edge();
        auto const sample_rate = this->_project_format.sample_rate;
        auto const &scale = display_space->scale();

        this->_locations->set_value({.begin = edge_location(edge.begin_frame, sample_rate, scale),
                                     .end = edge_location(edge.end_frame, sample_rate, scale)});
    } else {
        this->_locations->set_value({.begin = {0}, .end = {0}});
    }
}

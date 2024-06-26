//
//  grid_presenter.cpp
//

#include "grid_presenter.hpp"

#include <ae-core/global/hierarchy.h>
#include <ae-core/project/features/display_space.h>
#include <cpp-utils/lock.h>

#include <ae-core/project_editing/features/display_space_time_range/display_space_time_range.hpp>
#include <ae-core/project_editing/features/grid_updater.hpp>

using namespace yas;
using namespace yas::ae;

static std::vector<std::optional<grid_content>> const _empty_contents;

std::shared_ptr<grid_presenter> grid_presenter::make_shared(project_lifetime_id const lifetime_id) {
    auto const &project_lifetime = hierarchy::project_lifetime_for_id(lifetime_id);
    auto const &project_editing_lifetime = hierarchy::project_editing_lifetime_for_id(lifetime_id);
    return std::make_shared<grid_presenter>(
        lifetime_id, project_editing_lifetime->project_format, project_lifetime->display_space,
        project_editing_lifetime->display_space_time_range, project_editing_lifetime->grid_updater,
        project_editing_lifetime->grid_content_pool);
}

grid_presenter::grid_presenter(project_lifetime_id const lifetime_id, project_format const project_format,
                               std::shared_ptr<display_space> const &display_space,
                               std::shared_ptr<display_space_time_range> const &display_space_time_range,
                               std::shared_ptr<grid_updater> const &updater,
                               std::shared_ptr<grid_content_pool> const &content_pool)
    : _project_format(project_format),
      _display_space(display_space),
      _display_space_time_range(display_space_time_range),
      _updater(updater),
      _content_pool(content_pool) {
}

std::vector<std::optional<grid_content>> const &grid_presenter::contents() const {
    if (auto const content_pool = this->_content_pool.lock()) {
        return content_pool->elements();
    } else {
        return _empty_contents;
    }
}

void grid_presenter::update_contents() {
    auto const locked = yas::lock(this->_display_space_time_range, this->_display_space, this->_updater);
    if (!fulfilled(locked)) {
        return;
    }

    auto const &space_range = std::get<0>(locked);
    auto const &display_space = std::get<1>(locked);
    auto const &updater = std::get<2>(locked);

    if (auto const current_range = space_range->current(); current_range.has_value()) {
        updater->update_if_source_changed(
            {.begin_frame = current_range.value().frame,
             .end_frame = current_range.value().next_frame(),
             .min_interval = static_cast<length_t>(
                 std::ceilf(display_space->frames_per_point_width(this->_project_format.sample_rate) * 8.0f))});
    } else {
        updater->reset();
    }
}

observing::syncable grid_presenter::observe_contents(std::function<void(grid_content_pool_event const &)> &&handler) {
    if (auto const content_pool = this->_content_pool.lock()) {
        return content_pool->observe_event(std::move(handler));
    } else {
        return observing::syncable{};
    }
}

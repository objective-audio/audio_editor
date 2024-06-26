//
//  grid_presenter.hpp
//

#pragma once

#include <ae-core/app/value_types/project_format.h>
#include <ae-core/global/value_types/project_lifetime_id.h>
#include <ae-core/project_editing/features/grid_content_pool.h>

namespace yas::ae {
class display_space;
class display_space_time_range;
class grid_updater;
class grid_content;

struct grid_presenter final {
    [[nodiscard]] static std::shared_ptr<grid_presenter> make_shared(project_lifetime_id const);
    grid_presenter(project_lifetime_id const, project_format const, std::shared_ptr<display_space> const &,
                   std::shared_ptr<display_space_time_range> const &, std::shared_ptr<grid_updater> const &,
                   std::shared_ptr<grid_content_pool> const &);

    [[nodiscard]] std::vector<std::optional<grid_content>> const &contents() const;
    [[nodiscard]] observing::syncable observe_contents(std::function<void(grid_content_pool_event const &)> &&);

    void update_contents();

   private:
    project_format const _project_format;
    std::weak_ptr<display_space> const _display_space;
    std::weak_ptr<display_space_time_range> const _display_space_time_range;
    std::weak_ptr<grid_updater> const _updater;
    std::weak_ptr<grid_content_pool> const _content_pool;

    grid_presenter(grid_presenter const &) = delete;
    grid_presenter(grid_presenter &&) = delete;
    grid_presenter &operator=(grid_presenter const &) = delete;
    grid_presenter &operator=(grid_presenter &&) = delete;
};
}  // namespace yas::ae

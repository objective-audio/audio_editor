//
//  ae_project_pool.h
//

#pragma once

#include <audio_editor_core/ae_app_presenter_dependency.h>
#include <audio_editor_core/ae_project_pool_dependency.h>
#include <audio_editor_core/ae_project_pool_types.h>

namespace yas::ae {
class project;
class uuid_generator;

struct project_pool final : app_presenter_project_pool_interface {
    void add_project(url const &file_url) override;
    std::shared_ptr<project> add_and_return_project(url const &file_url);
    [[nodiscard]] std::shared_ptr<project> project_for_id(std::string const &) const;
    [[nodiscard]] std::size_t size() const;
    [[nodiscard]] observing::syncable observe_event(std::function<void(project_pool_event const &)> &&) override;

    static std::shared_ptr<project_pool> make_shared();
    static std::shared_ptr<project_pool> make_shared(std::shared_ptr<project_pool_uuid_generator_interface> const &);

   private:
    std::shared_ptr<project_pool_uuid_generator_interface> const _uuid_generator;
    using projects_map_t =
        observing::map::holder<std::string, std::pair<std::shared_ptr<project>, observing::cancellable_ptr>>;
    std::shared_ptr<projects_map_t> const _projects = projects_map_t::make_shared();

    project_pool(std::shared_ptr<project_pool_uuid_generator_interface> const &);

    project_pool(project_pool const &) = delete;
    project_pool(project_pool &&) = delete;
    project_pool &operator=(project_pool const &) = delete;
    project_pool &operator=(project_pool &&) = delete;
};
}  // namespace yas::ae

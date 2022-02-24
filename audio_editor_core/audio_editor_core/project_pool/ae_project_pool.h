//
//  ae_project_pool.h
//

#pragma once

#include <audio_editor_core/ae_app_presenter_dependency.h>
#include <audio_editor_core/ae_project_pool_dependency.h>
#include <audio_editor_core/ae_project_pool_types.h>

namespace yas::ae {
class project_maker;
class project;
class project_level;

struct project_pool final : project_pool_for_app_presenter {
    void add_project_level(url const &file_url) override;
    std::shared_ptr<project_level> add_and_return_project_level(url const &file_url);
    [[nodiscard]] std::shared_ptr<project_level> const &project_level_for_id(std::string const &) const;
    [[nodiscard]] std::size_t size() const;
    [[nodiscard]] observing::syncable observe_event(std::function<void(project_pool_event const &)> &&) override;

    [[nodiscard]] static std::shared_ptr<project_pool> make_shared();
    [[nodiscard]] static std::shared_ptr<project_pool> make_shared(std::shared_ptr<project_maker> const &);

   private:
    std::shared_ptr<project_maker> const _project_maker;

    using project_levels_t =
        observing::map::holder<std::string, std::pair<std::shared_ptr<project_level>, observing::cancellable_ptr>>;
    std::shared_ptr<project_levels_t> const _project_levels = project_levels_t::make_shared();

    project_pool(std::shared_ptr<project_maker> const &);

    project_pool(project_pool const &) = delete;
    project_pool(project_pool &&) = delete;
    project_pool &operator=(project_pool const &) = delete;
    project_pool &operator=(project_pool &&) = delete;
};
}  // namespace yas::ae

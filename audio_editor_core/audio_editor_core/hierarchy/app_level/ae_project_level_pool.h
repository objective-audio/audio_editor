//
//  ae_project_level_pool.h
//

#pragma once

#include <audio_editor_core/ae_app_presenter_dependency.h>
#include <audio_editor_core/ae_project_level_pool_types.h>

namespace yas::ae {
class project_level;
class uuid_generatable;

struct project_level_pool final : project_pool_for_app_presenter {
    void add_level(url const &file_url) override;
    std::shared_ptr<project_level> add_and_return_level(url const &file_url);
    [[nodiscard]] std::shared_ptr<project_level> const &level_for_id(std::string const &) const;
    [[nodiscard]] std::size_t size() const;
    [[nodiscard]] observing::syncable observe_event(std::function<void(project_level_pool_event const &)> &&) override;

    [[nodiscard]] static std::shared_ptr<project_level_pool> make_shared();
    [[nodiscard]] static std::shared_ptr<project_level_pool> make_shared(std::shared_ptr<uuid_generatable> const &);

   private:
    std::shared_ptr<uuid_generatable> const _uuid_generator;

    using project_levels_t =
        observing::map::holder<std::string, std::pair<std::shared_ptr<project_level>, observing::cancellable_ptr>>;
    std::shared_ptr<project_levels_t> const _project_levels = project_levels_t::make_shared();

    project_level_pool(std::shared_ptr<uuid_generatable> const &);

    project_level_pool(project_level_pool const &) = delete;
    project_level_pool(project_level_pool &&) = delete;
    project_level_pool &operator=(project_level_pool const &) = delete;
    project_level_pool &operator=(project_level_pool &&) = delete;
};
}  // namespace yas::ae

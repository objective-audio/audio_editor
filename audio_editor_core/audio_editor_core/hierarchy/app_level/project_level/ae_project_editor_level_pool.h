//
//  ae_project_editor_level_pool.h
//

#pragma once

#include <audio_editor_core/ae_project_dependency.h>
#include <observing/yas_observing_umbrella.h>

#include <memory>

namespace yas::ae {
class project_editor_level;

struct project_editor_level_pool final : project_editor_level_pool_for_project {
    [[nodiscard]] static std::shared_ptr<project_editor_level_pool> make_shared(std::string const &identifier);

    void add_level(file_info const &) override;
    void remove_level() override;

    std::shared_ptr<project_editor_level> const &level() const;

    observing::syncable observe_level(std::function<void(std::shared_ptr<project_editor_level> const &)> &&);

   private:
    std::string const _identifier;

    observing::value::holder_ptr<std::shared_ptr<project_editor_level>> const _editor_level;

    project_editor_level_pool(std::string const &identifier);
};
}  // namespace yas::ae

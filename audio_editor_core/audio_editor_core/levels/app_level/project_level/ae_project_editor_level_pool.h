//
//  ae_project_editor_level_pool.h
//

#pragma once

#include <audio_editor_core/ae_project_closer_dependency.h>
#include <audio_editor_core/ae_project_launcher_dependency.h>
#include <observing/yas_observing_umbrella.h>

namespace yas::ae {
class project_editor_level;

struct project_editor_level_pool final : project_editor_level_pool_for_project_launcher,
                                         project_editor_level_pool_for_project_closer {
    [[nodiscard]] static std::shared_ptr<project_editor_level_pool> make_shared(project_id const &);

    void add_level(project_format const &, file_info const &) override;
    void remove_level() override;

    [[nodiscard]] std::shared_ptr<project_editor_level> const &level() const override;

    observing::syncable observe_level(std::function<void(std::shared_ptr<project_editor_level> const &)> &&);

   private:
    project_id const _project_id;

    observing::value::holder_ptr<std::shared_ptr<project_editor_level>> const _editor_level;

    project_editor_level_pool(project_id const &);
};
}  // namespace yas::ae

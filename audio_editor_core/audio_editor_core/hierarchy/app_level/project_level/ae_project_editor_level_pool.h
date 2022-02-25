//
//  ae_project_editor_level_pool.h
//

#pragma once

#include <audio_editor_core/ae_project_dependency.h>

#include <memory>

namespace yas::ae {
class project_editor_level;
class project_editor_level_maker;

struct project_editor_level_pool final : project_editor_level_pool_for_project {
    [[nodiscard]] static std::shared_ptr<project_editor_level_pool> make_shared(
        std::shared_ptr<project_editor_level_maker> const &);

    void add_editor_level(file_info const &) override;
    std::shared_ptr<project_editor_level> const &editor_level() const;

   private:
    std::shared_ptr<project_editor_level_maker> const _maker;
    std::shared_ptr<project_editor_level> _editor_level;

    project_editor_level_pool(std::shared_ptr<project_editor_level_maker> const &);
};
}  // namespace yas::ae

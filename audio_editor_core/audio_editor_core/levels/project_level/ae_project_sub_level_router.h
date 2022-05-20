//
//  ae_project_sub_level_router.h
//

#pragma once

#include <audio_editor_core/ae_project_id.h>
#include <audio_editor_core/ae_project_sub_level.h>
#include <observing/yas_observing_umbrella.h>

namespace yas::ae {
struct project_sub_level_router final {
    [[nodiscard]] static std::shared_ptr<project_sub_level_router> make_shared(project_id const &);

    project_sub_level_router(project_id const &);

    void add_time_editor(number_components const &, std::optional<std::size_t> const unit_idx);
    void remove_time_editor();
    std::shared_ptr<time_editor_level> const &time_editor_level() const;

    void add_sheet(sheet_content const &);
    void remove_sheet();
    std::shared_ptr<sheet_level> const &sheet_level() const;

    [[nodiscard]] observing::syncable observe(std::function<void(std::optional<project_sub_level> const &)> &&);

   private:
    project_id const _project_id;

    observing::value::holder_ptr<std::optional<project_sub_level>> const _sub_level;
};
}  // namespace yas::ae

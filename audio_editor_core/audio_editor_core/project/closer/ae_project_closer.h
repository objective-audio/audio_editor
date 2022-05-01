//
//  ae_project_closer.h
//

#pragma once

#include <audio_editor_core/ae_project_closer_dependency.h>
#include <audio_editor_core/ae_project_types.h>
#include <audio_editor_core/ae_window_presenter_dependency.h>
#include <observing/yas_observing_umbrella.h>

namespace yas::ae {
class file_importer;
class project_editor_level_pool;
class project_status;

struct project_closer final : project_closer_for_window_presenter {
    [[nodiscard]] static std::shared_ptr<project_closer> make_shared(
        std::string const &project_id, std::shared_ptr<file_importer_for_project_closer> const &,
        std::shared_ptr<project_level_pool_for_project_closer> const &,
        std::shared_ptr<project_editor_level_pool_for_project_closer> const &,
        std::shared_ptr<project_status_for_project_closer> const &);

    [[nodiscard]] bool can_close() const override;
    void request_close() override;

   private:
    std::string const _project_id;

    std::weak_ptr<file_importer_for_project_closer> const _file_importer;
    std::weak_ptr<project_level_pool_for_project_closer> const _project_level_pool;
    std::weak_ptr<project_editor_level_pool_for_project_closer> const _editor_level_pool;
    std::weak_ptr<project_status_for_project_closer> const _status;

    project_closer(std::string const &project_id, std::shared_ptr<file_importer_for_project_closer> const &,
                   std::shared_ptr<project_level_pool_for_project_closer> const &,
                   std::shared_ptr<project_editor_level_pool_for_project_closer> const &,
                   std::shared_ptr<project_status_for_project_closer> const &);
};
}  // namespace yas::ae

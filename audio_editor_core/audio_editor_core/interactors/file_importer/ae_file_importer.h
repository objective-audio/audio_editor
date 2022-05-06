//
//  ae_file_importer.h
//

#pragma once

#include <audio_editor_core/ae_project_closer_dependency.h>
#include <audio_editor_core/ae_project_id.h>
#include <audio_editor_core/ae_project_launcher_dependency.h>
#include <cpp_utils/yas_worker.h>

namespace yas::ae {
class file_importer_resource;

struct file_importer final : file_importer_for_project_launcher, file_importer_for_project_closer {
    void import(file_importing_context &&) override;
    void cancel(project_id const &) override;

    [[nodiscard]] static std::shared_ptr<file_importer> make_shared(workable_ptr const &, uint32_t const priority);

   private:
    workable_ptr _worker;
    std::shared_ptr<file_importer_resource> _resource;
    uint32_t const _priority;

    file_importer(workable_ptr const &, uint32_t const);

    file_importer(file_importer const &) = delete;
    file_importer(file_importer &&) = delete;
    file_importer &operator=(file_importer const &) = delete;
    file_importer &operator=(file_importer &&) = delete;
};
}  // namespace yas::ae

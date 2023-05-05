//
//  ae_file_importer.h
//

#pragma once

#include <audio_editor_core/ae_file_importer_types.h>
#include <audio_editor_core/ae_project_guid.h>
#include <cpp_utils/yas_worker.h>

namespace yas::ae {
class file_importer_resource;

struct file_importer final {
    file_importer(workable_ptr const &, uint32_t const);

    void import(file_importing_context &&);
    void cancel(project_guid const &);

   private:
    workable_ptr _worker;
    std::shared_ptr<file_importer_resource> _resource;
    uint32_t const _priority;

    file_importer(file_importer const &) = delete;
    file_importer(file_importer &&) = delete;
    file_importer &operator=(file_importer const &) = delete;
    file_importer &operator=(file_importer &&) = delete;
};
}  // namespace yas::ae

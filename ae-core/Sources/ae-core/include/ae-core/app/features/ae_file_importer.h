//
//  ae_file_importer.h
//

#pragma once

#include <ae-core/app/value_types/ae_file_importer_types.h>
#include <ae-core/global/value_types/ae_project_guid.h>
#include <cpp-utils/worker.h>

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

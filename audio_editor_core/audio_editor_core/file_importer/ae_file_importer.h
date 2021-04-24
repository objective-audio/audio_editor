//
//  ae_file_importer.h
//

#pragma once

#include <cpp_utils/yas_task.h>
#include <cpp_utils/yas_url.h>

#include <functional>
#include <memory>

namespace yas::ae {
struct file_importer {
    void import(url const &src_url, url const &dst_url, std::function<void(bool const)> &&completion);

    static std::shared_ptr<file_importer> make_shared(task_queue const &, task_priority_t const);

   private:
    task_queue _queue;
    task_priority_t const _priority;

    file_importer(task_queue const &, task_priority_t const);

    file_importer(file_importer const &) = delete;
    file_importer(file_importer &&) = delete;
    file_importer &operator=(file_importer const &) = delete;
    file_importer &operator=(file_importer &&) = delete;
};
}  // namespace yas::ae

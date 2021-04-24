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

    static std::shared_ptr<file_importer> make_shared();

   private:
    task_queue _queue;

    file_importer() = default;

    file_importer(file_importer const &) = delete;
    file_importer(file_importer &&) = delete;
    file_importer &operator=(file_importer const &) = delete;
    file_importer &operator=(file_importer &&) = delete;
};
}  // namespace yas::ae

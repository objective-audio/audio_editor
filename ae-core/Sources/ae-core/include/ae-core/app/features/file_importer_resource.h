//
//  file_importer_resource.h
//

#pragma once

#include <ae-core/app/value_types/file_importer_types.h>
#include <ae-core/global/value_types/project_guid.h>

#include <deque>
#include <mutex>
#include <optional>
#include <vector>

namespace yas::ae {
struct file_importer_resource final {
    file_importer_resource() = default;

    void push_context_on_main(file_importing_context &&);
    void cancel_on_main(project_guid const &project_id);

    [[nodiscard]] std::optional<file_importing_context> pull_context_on_task();
    [[nodiscard]] std::vector<std::string> pull_cancel_ids();

   private:
    std::mutex _mutex;
    std::deque<file_importing_context> _contexts;
    std::vector<std::string> _cancel_ids;

    file_importer_resource(file_importer_resource const &) = delete;
    file_importer_resource(file_importer_resource &&) = delete;
    file_importer_resource &operator=(file_importer_resource const &) = delete;
    file_importer_resource &operator=(file_importer_resource &&) = delete;
};
}  // namespace yas::ae

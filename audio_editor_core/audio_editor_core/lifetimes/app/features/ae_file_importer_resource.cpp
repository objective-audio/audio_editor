//
//  ae_file_importer_resource.cpp
//

#include "ae_file_importer_resource.h"

#include <cpp_utils/yas_stl_utils.h>

using namespace yas;
using namespace yas::ae;

void file_importer_resource::push_context_on_main(file_importing_context &&context) {
    std::lock_guard<std::mutex> lock(this->_mutex);
    this->_contexts.emplace_back(std::move(context));
}

void file_importer_resource::cancel_on_main(project_id const &project_id) {
    std::lock_guard<std::mutex> lock(this->_mutex);
    std::erase_if(this->_contexts,
                  [&project_id](file_importing_context const &context) { return context.project_id == project_id; });
    this->_cancel_ids.emplace_back(project_id.raw_value);
}

std::optional<file_importing_context> file_importer_resource::pull_context_on_task() {
    if (auto lock = std::unique_lock<std::mutex>(this->_mutex, std::try_to_lock); lock.owns_lock()) {
        this->_cancel_ids.clear();

        if (!this->_contexts.empty()) {
            std::optional<file_importing_context> result = std::move(this->_contexts.front());
            this->_contexts.pop_front();
            return result;
        }
    }

    return std::nullopt;
}

std::vector<std::string> file_importer_resource::pull_cancel_ids() {
    if (auto lock = std::unique_lock<std::mutex>(this->_mutex, std::try_to_lock); lock.owns_lock()) {
        std::vector<std::string> result = std::move(this->_cancel_ids);
        this->_cancel_ids.clear();
        return result;
    }

    return {};
}

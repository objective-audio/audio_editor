//
//  ae_project_opener.cpp
//

#include "ae_project_opener.h"

#include <audio_editor_core/ae_file_info_loader.h>
#include <audio_editor_core/ae_hierarchy.h>
#include <audio_editor_core/ae_project_lifecycle.h>
#include <cpp_utils/yas_assertion.h>
#include <cpp_utils/yas_file_manager.h>

using namespace yas;
using namespace yas::ae;

project_opener::project_opener(file_info_loader const *file_info_loader, project_lifecycle *lifecycle)
    : _file_info_loader(file_info_loader), _project_lifecycle(lifecycle) {
}

void project_opener::open(project_format const &format, std::filesystem::path const &project_path) {
    auto const exists = file_manager::content_exists(project_path);
    if (exists.is_success()) {
        switch (exists.value()) {
            case file_manager::content_kind::directory:
                break;
            case file_manager::content_kind::file:
                assertion_failure_if_not_test();
                return;
        }
    } else {
        assertion_failure_if_not_test();
        return;
    }

    this->_project_lifecycle->add_lifetime(project_path, format);
}

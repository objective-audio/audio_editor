//
//  ae_project_preparer.cpp
//

#include "ae_project_preparer.h"

#include <audio_editor_core/ae_file_info_loader.h>
#include <audio_editor_core/ae_hierarchy.h>
#include <audio_editor_core/ae_system_url.h>
#include <audio_editor_core/ae_uuid_generator.h>
#include <audio_editor_core/ae_window_lifecycle.h>
#include <cpp_utils/yas_assertion.h>
#include <cpp_utils/yas_file_manager.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<project_preparer> project_preparer::make_shared(file_info_loader const *file_info_loader,
                                                                window_lifecycle *lifecycle) {
    return std::make_shared<project_preparer>(uuid_generator::make_shared(), file_info_loader, lifecycle);
}

project_preparer::project_preparer(std::shared_ptr<uuid_generatable> const &uuid_generator,
                                   file_info_loader const *file_info_loader, window_lifecycle *lifecycle)
    : _uuid_generator(uuid_generator), _file_info_loader(file_info_loader), _window_lifecycle(lifecycle) {
}

void project_preparer::prepare(project_format const &format, url const &project_url) {
    auto const identifier = this->_uuid_generator->generate();
    project_id const project_id{.raw_value = identifier};

    auto const exists = file_manager::content_exists(project_url.path());
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

    if (this->_window_lifecycle->lifetime_for_id(project_id) != nullptr) {
        assertion_failure_if_not_test();
        return;
    }

    this->_window_lifecycle->add_lifetime(project_url, project_id, format);
}

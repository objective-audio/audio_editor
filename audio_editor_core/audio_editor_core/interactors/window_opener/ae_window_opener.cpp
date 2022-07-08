//
//  ae_window_opener.cpp
//

#include "ae_window_opener.h"

#include <audio_editor_core/ae_file_info_loader.h>
#include <audio_editor_core/ae_hierarchy.h>
#include <audio_editor_core/ae_system_url.h>
#include <audio_editor_core/ae_uuid_generator.h>
#include <audio_editor_core/ae_window_lifecycle.h>
#include <cpp_utils/yas_assertion.h>
#include <cpp_utils/yas_file_manager.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<window_opener> window_opener::make_shared(file_info_loader const *file_info_loader,
                                                          window_lifecycle *lifecycle) {
    return std::make_shared<window_opener>(uuid_generator::make_shared(), file_info_loader, lifecycle);
}

window_opener::window_opener(std::shared_ptr<uuid_generatable> const &uuid_generator,
                             file_info_loader const *file_info_loader, window_lifecycle *lifecycle)
    : _uuid_generator(uuid_generator), _file_info_loader(file_info_loader), _window_lifecycle(lifecycle) {
}

void window_opener::open(project_format const &format, url const &project_url) {
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

    if (this->_window_lifecycle->has_lifetime_for_project_id(project_id)) {
        assertion_failure_if_not_test();
        return;
    }

    this->_window_lifecycle->add_lifetime(project_url, project_id, format);
}

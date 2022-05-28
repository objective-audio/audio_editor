//
//  ae_project_preparer.cpp
//

#include "ae_project_preparer.h"

#include <audio_editor_core/ae_file_info_loader.h>
#include <audio_editor_core/ae_hierarchy.h>
#include <audio_editor_core/ae_project_level_router.h>
#include <audio_editor_core/ae_system_url.h>
#include <audio_editor_core/ae_uuid_generator.h>
#include <cpp_utils/yas_assertion.h>
#include <cpp_utils/yas_file_manager.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<project_preparer> project_preparer::make_shared(file_info_loader const *file_info_loader,
                                                                ae::system_url const *system_url,
                                                                project_level_router *router) {
    return std::make_shared<project_preparer>(uuid_generator::make_shared(), system_url, file_info_loader, router);
}

project_preparer::project_preparer(std::shared_ptr<uuid_generatable> const &uuid_generator,
                                   ae::system_url const *system_url, file_info_loader const *file_info_loader,
                                   project_level_router *router)
    : _uuid_generator(uuid_generator),
      _system_url(system_url),
      _file_info_loader(file_info_loader),
      _project_level_router(router) {
}

void project_preparer::prepare(url const &file_url) {
    auto const file_info = this->_file_info_loader->load_file_info(file_url);
    if (!file_info.has_value()) {
        assertion_failure_if_not_test();
        return;
    }

    project_format const format{.sample_rate = file_info.value().sample_rate,
                                .channel_count = file_info.value().channel_count};

    this->prepare(format, file_url);
}

void project_preparer::prepare(project_format const &format, url const &file_url) {
    auto const identifier = this->_uuid_generator->generate();
    project_id const project_id{.raw_value = identifier};

    auto const result =
        file_manager::create_directory_if_not_exists(this->_system_url->project_directory(project_id).path());
    if (result.is_error()) {
        assertion_failure_if_not_test();
        return;
    }

    if (this->_project_level_router->level_for_id(project_id) != nullptr) {
        assertion_failure_if_not_test();
        return;
    }

    this->_project_level_router->add_level(file_url, project_id, format);
}

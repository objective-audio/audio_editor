//
//  ae_project_maker.cpp
//

#include "ae_project_maker.h"

#include <audio_editor_core/ae_project_level.h>
#include <audio_editor_core/ae_uuid_generator.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<project_maker> project_maker::make_shared() {
    return make_shared(uuid_generator::make_shared());
}

std::shared_ptr<project_maker> project_maker::make_shared(std::shared_ptr<uuid_generatable> const &uuid_generator) {
    return std::shared_ptr<project_maker>(new project_maker{uuid_generator});
}

project_maker::project_maker(std::shared_ptr<uuid_generatable> const &uuid_generator)
    : _uuid_generator(uuid_generator) {
}

std::shared_ptr<project> project_maker::make(url const &file_url) {
    auto const identifier = this->_uuid_generator->generate();

    auto const project_level = project_level::make_shared(identifier, file_url);
    return project_level->project;
}

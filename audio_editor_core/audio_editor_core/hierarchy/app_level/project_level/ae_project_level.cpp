//
//  ae_project_level.cpp
//

#include "ae_project_level.h"

using namespace yas;
using namespace yas::ae;

std::shared_ptr<project_level> project_level::make_shared(std::string const &identifier) {
    return std::shared_ptr<project_level>(new project_level{identifier});
}

project_level::project_level(std::string const &identifier) : identifier(identifier) {
}

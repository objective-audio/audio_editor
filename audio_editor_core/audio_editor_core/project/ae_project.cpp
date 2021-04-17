//
//  ae_project.cpp
//

#include "ae_project.h"

using namespace yas;
using namespace yas::ae;

project::project() {
}

project_ptr project::make_shared() {
    return std::shared_ptr<project>(new project{});
}

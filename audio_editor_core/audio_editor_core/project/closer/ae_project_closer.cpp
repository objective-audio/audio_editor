//
//  ae_project_closer.cpp
//

#include "ae_project_closer.h"

using namespace yas;
using namespace yas::ae;

std::shared_ptr<project_closer> project_closer::make_shared() {
    return std::shared_ptr<project_closer>(new project_closer{});
}

project_closer::project_closer() {
}

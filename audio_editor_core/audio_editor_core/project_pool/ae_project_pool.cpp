//
//  ae_project_pool.cpp
//

#include "ae_project_pool.h"

using namespace yas;
using namespace yas::ae;

project_pool::project_pool() {
}

project_pool_ptr project_pool::make_shared() {
    return std::shared_ptr<project_pool>(new project_pool{});
}

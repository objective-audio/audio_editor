//
//  ae_time_editor_level_pool.cpp
//

#include "ae_time_editor_level_pool.h"

using namespace yas;
using namespace yas::ae;

std::shared_ptr<time_editor_level_pool> time_editor_level_pool::make_shared() {
    return std::shared_ptr<time_editor_level_pool>(new time_editor_level_pool{});
}

time_editor_level_pool::time_editor_level_pool() {
}

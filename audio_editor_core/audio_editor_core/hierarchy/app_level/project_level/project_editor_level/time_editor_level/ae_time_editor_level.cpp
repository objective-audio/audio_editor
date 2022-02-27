//
//  ae_time_editor_level.cpp
//

#include "ae_time_editor_level.h"

using namespace yas;
using namespace yas::ae;

std::shared_ptr<time_editor_level> time_editor_level::make_shared(std::shared_ptr<ae::time_editor> const &time_editor) {
    return std::shared_ptr<time_editor_level>(new time_editor_level{time_editor});
}

time_editor_level::time_editor_level(std::shared_ptr<ae::time_editor> const &time_editor) : time_editor(time_editor) {
}

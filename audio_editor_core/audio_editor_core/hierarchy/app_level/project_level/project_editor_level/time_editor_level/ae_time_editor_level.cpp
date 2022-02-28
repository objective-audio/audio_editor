//
//  ae_time_editor_level.cpp
//

#include "ae_time_editor_level.h"

#include <audio_editor_core/ae_time_editor.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<time_editor_level> time_editor_level::make_shared(number_components const &components) {
    return std::shared_ptr<time_editor_level>(new time_editor_level{components});
}

time_editor_level::time_editor_level(number_components const &components)
    : time_editor(time_editor::make_shared(components)) {
}

//
//  ae_time_editor_maker.cpp
//

#include "ae_time_editor_maker.h"

#include <audio_editor_core/ae_time_editor.h>
#include <audio_editor_core/ae_time_editor_level.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<time_editor_maker> time_editor_maker::make_shared() {
    return std::shared_ptr<time_editor_maker>(new time_editor_maker{});
}

std::shared_ptr<time_editor_level> time_editor_maker::make(number_components const &components) const {
    return time_editor_level::make_shared(components);
}

time_editor_maker::time_editor_maker() {
}

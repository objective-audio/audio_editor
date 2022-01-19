//
//  ae_time_editor_maker.cpp
//

#include "ae_time_editor_maker.h"

#include <audio_editor_core/ae_time_editor.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<time_editor_maker> time_editor_maker::make_shared() {
    return std::shared_ptr<time_editor_maker>(new time_editor_maker{});
}

std::shared_ptr<time_editor_for_project_editor> time_editor_maker::make() const {
    return time_editor::make_shared();
}

time_editor_maker::time_editor_maker() {
}

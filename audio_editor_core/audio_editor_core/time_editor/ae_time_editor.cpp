//
//  ae_time_editor.cpp
//

#include "ae_time_editor.h"

using namespace yas;
using namespace yas::ae;

std::shared_ptr<time_editor> time_editor::make_shared() {
    return std::shared_ptr<time_editor>(new time_editor{});
}

time_editor::time_editor() {
}

void time_editor::input_number(uint32_t const number) {
}

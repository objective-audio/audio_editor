//
//  ae_ui_types.cpp
//

#include "ae_ui_types.h"

using namespace yas;
using namespace yas::ae;

void ae::line_index2d_rect::set_all(ui::index2d_t const first) {
    this->v[0] = this->v[7] = first;
    this->v[1] = this->v[2] = first + 1;
    this->v[3] = this->v[4] = first + 3;
    this->v[5] = this->v[6] = first + 2;
}

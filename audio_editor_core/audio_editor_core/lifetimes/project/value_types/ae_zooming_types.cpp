//
//  ae_zooming_size.cpp
//

#include "ae_zooming_types.hpp"

#include <audio_editor_core/ae_ui_zooming_constants.h>

using namespace yas;
using namespace yas::ae;

zooming_size yas::to_zooming_size(ae::zooming_scale const &scale) {
    return {.width_per_sec = ui_zooming_constants::standard_width_per_sec * scale.horizontal,
            .height_per_track = ui_zooming_constants::standard_height_per_track * scale.vertical};
}

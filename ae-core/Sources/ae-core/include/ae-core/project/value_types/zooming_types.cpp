//
//  zooming_size.cpp
//

#include "zooming_types.hpp"

#include <ae-core/ui_base/features/zooming/ui_zooming_constants.h>

using namespace yas;
using namespace yas::ae;

zooming_size yas::to_zooming_size(ae::zooming_scale const &scale) {
    return {.width_per_sec = ui_zooming_constants::standard_width_per_sec * scale.horizontal,
            .height_per_track = ui_zooming_constants::standard_height_per_track * scale.vertical};
}

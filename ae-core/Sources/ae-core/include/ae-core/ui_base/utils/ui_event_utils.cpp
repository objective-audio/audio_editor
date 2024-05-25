//
//  ui_event_utils.cpp
//

#include "ui_event_utils.h"

#include <cpp-utils/stl_utils.h>

using namespace yas;
using namespace yas::ae;

std::string ui_event_utils::to_event_string(ui::point const &point) {
    return std::to_string(point.x) + "," + std::to_string(point.y);
}

ui::point ui_event_utils::to_position_from_event_string(std::string const &string) {
    auto const splited = yas::split(string, ',');

    if (splited.size() != 2) {
        return ui::point::zero();
    }

    return {.x = std::stof(splited.at(0)), .y = std::stof(splited.at(1))};
}

//
//  ae_ui_button_utils.cpp
//

#include "ae_ui_button_utils.h"

#include <cpp_utils/yas_stl_utils.h>

#include <type_traits>

using namespace yas;
using namespace yas::ae;

std::size_t ui_button_utils::to_state_idx(ui_button_state const state) {
    return static_cast<std::underlying_type_t<ui_button_state>>(state);
}

std::function<bool(std::shared_ptr<ui::event> const &)> ui_button_utils::is_touch_accepted(
    std::set<ui::touch_id> &&touch_ids) {
    return [touch_ids = std::move(touch_ids)](std::shared_ptr<ui::event> const &event) {
        auto const touch_event = event->get<ui::touch>();
        return touch_ids.contains(touch_event.touch_id);
    };
}

std::string ui_button_utils::to_event_string(ui::point const &point) {
    return std::to_string(point.x) + "," + std::to_string(point.y);
}

ui::point ui_button_utils::to_position_from_event_string(std::string const &string) {
    auto const splited = yas::split(string, ',');

    if (splited.size() != 2) {
        return ui::point::zero();
    }

    return {.x = std::stof(splited.at(0)), .y = std::stof(splited.at(1))};
}

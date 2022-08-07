//
//  ae_ui_playing_line.cpp
//

#include "ae_ui_playing_line.hpp"
#include <audio_editor_core/ae_color.h>
#include <audio_editor_core/ae_hierarchy.h>
#include <audio_editor_core/ae_ui_hierarchy.h>
#include <audio_editor_core/ae_ui_playing_line_utils.h>
#include <audio_editor_core/ae_playing_line_presenter.hpp>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<ui_playing_line> ui_playing_line::make_shared(window_lifetime_id const &lifetime_id,
                                                              std::shared_ptr<ui::node> const &node) {
    auto const &app_lifetime = hierarchy::app_lifetime();
    auto const &resource_lifetime = ui_hierarchy::resource_lifetime_for_window_lifetime_id(lifetime_id);
    auto const presenter = playing_line_presenter::make_shared(lifetime_id);

    return std::make_shared<ui_playing_line>(app_lifetime->color.get(), presenter, resource_lifetime->standard, node);
}

ui_playing_line::ui_playing_line(ae::color *color, std::shared_ptr<playing_line_presenter> const &presenter,
                                 std::shared_ptr<ui::standard> const &standard, std::shared_ptr<ui::node> const &node)
    : _color(color), _presenter(presenter), _playing_line(ui::rect_plane::make_shared(1)) {
    node->add_sub_node(this->_playing_line->node());

    standard->renderer()
        ->observe_will_render([this](auto const &) {
            this->_playing_line->node()->set_color(
                ui_playing_line_utils::to_playing_line_color(this->_presenter->playing_line_state(), this->_color));
        })
        .end()
        ->add_to(this->_pool);

    standard->view_look()
        ->safe_area_layout_guide()
        ->observe([this](ui::region const &src_region) {
            ui::region const dst_region{.origin = {.x = src_region.center().x - 0.5f, .y = src_region.origin.y},
                                        .size = {.width = 1.0f, .height = src_region.size.height}};
            this->_playing_line->data()->set_rect_position(dst_region, 0);
        })
        .sync()
        ->add_to(this->_pool);
}

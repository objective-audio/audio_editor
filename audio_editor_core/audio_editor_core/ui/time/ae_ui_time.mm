//
//  ae_ui_time.mm
//

#include "ae_ui_time.h"
#include <audio_editor_core/ae_time_presenter.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<ui_time> ui_time::make_shared(std::shared_ptr<ui::standard> const &standard,
                                              std::shared_ptr<ui::texture> const &texture,
                                              std::string const &project_id) {
    auto const presenter = time_presenter::make_shared(project_id);
    return std::shared_ptr<ui_time>(new ui_time{standard, texture, presenter});
}

ui_time::ui_time(std::shared_ptr<ui::standard> const &standard, std::shared_ptr<ui::texture> const &texture,
                 std::shared_ptr<time_presenter> const &presenter)
    : _presenter(presenter),
      _font_atlas(ui::font_atlas::make_shared(
          {.font_name = "TrebuchetMS-Bold", .font_size = 26.0f, .words = " 1234567890.:"}, texture)),
      _top_guide(ui::layout_value_guide::make_shared()),
      _node(ui::node::make_shared()),
      _bg(ui::rect_plane::make_shared(1)),
      _time_strings(ui::strings::make_shared({.alignment = ui::layout_alignment::mid}, _font_atlas)) {
    this->_node->add_sub_node(this->_bg->node());
    this->_node->add_sub_node(this->_time_strings->rect_plane()->node());

    this->_bg->node()->set_color(ui::black_color());
    this->_bg->node()->set_alpha(0.5f);

    this->_node->attach_y_layout_guide(*this->_top_guide);

    this->_time_strings->actual_layout_source()
        ->observe_layout_region([this](ui::region const &region) { this->_bg->data()->set_rect_position(region, 0); })
        .sync()
        ->add_to(this->_pool);

    standard->renderer()
        ->observe_will_render([this](auto const &) { this->_time_strings->set_text(this->_presenter->time_text()); })
        .end()
        ->add_to(this->_pool);
}

std::shared_ptr<ui::layout_value_target> ui_time::top_layout_target() const {
    return this->_top_guide;
}

std::shared_ptr<ui::node> const &ui_time::node() const {
    return this->_node;
}

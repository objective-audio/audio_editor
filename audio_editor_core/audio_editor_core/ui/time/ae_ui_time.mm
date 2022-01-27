//
//  ae_ui_time.mm
//

#include "ae_ui_time.h"
#include <audio_editor_core/ae_time_presenter.h>
#include <audio_editor_core/ae_ui_types.h>
#include <cpp_utils/yas_fast_each.h>

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
          {.font_name = "TrebuchetMS-Bold", .font_size = 26.0f, .words = " 1234567890.:+-"}, texture)),
      _top_guide(ui::layout_value_guide::make_shared()),
      _node(ui::node::make_shared()),
      _bg(ui::rect_plane::make_shared(1)),
      _time_strings(ui::strings::make_shared({.alignment = ui::layout_alignment::mid}, _font_atlas)),
      _editing_frame(ui::rect_plane::make_shared(ui::rect_plane_data::make_shared(
          ui::dynamic_mesh_vertex_data::make_shared(vertex2d_rect::vector_count),
          ui::dynamic_mesh_index_data::make_shared(line_index2d_rect::vector_count)))) {
    this->_node->add_sub_node(this->_bg->node());
    this->_node->add_sub_node(this->_time_strings->rect_plane()->node());
    this->_node->add_sub_node(this->_editing_frame->node());

    this->_bg->node()->set_color(ui::black_color());
    this->_bg->node()->set_alpha(0.5f);

    auto const &editing_frame_data = this->_editing_frame->data();
    editing_frame_data->set_rect_position({.origin = {0.0f, 0.0f}, .size = {1.0f, 1.0f}}, 0);
    editing_frame_data->dynamic_index_data()->write([](std::vector<ui::index2d_t> &indices) {
        assert(indices.size() == line_index2d_rect::vector_count);
        auto *index_rects = (line_index2d_rect *)indices.data();
        index_rects[0].set_all(0);
    });

    auto const &editing_node = this->_editing_frame->node();
    editing_node->mesh()->set_primitive_type(ui::primitive_type::line);
    editing_node->set_is_enabled(false);

    this->_node->attach_y_layout_guide(*this->_top_guide);

    this->_time_strings->actual_layout_source()
        ->observe_layout_region([this](ui::region const &region) { this->_bg->data()->set_rect_position(region, 0); })
        .sync()
        ->add_to(this->_pool);

    // 最初の同期は次のobserve_editing_time_text_rangeに任せてend
    this->_time_strings
        ->observe_actual_cell_regions(
            [this](const std::vector<ui::region> &cell_regions) { this->_update_editing_frame(); })
        .end()
        ->add_to(this->_pool);

    this->_presenter->observe_editing_time_text_range([this](auto const &) { this->_update_editing_frame(); })
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

void ui_time::_update_editing_frame() {
    if (auto const range = this->_presenter->editing_time_text_range()) {
        auto const &range_value = range.value();

        std::optional<ui::region> region = std::nullopt;

        auto each = make_fast_each(range_value.index, range_value.index + range_value.length);
        while (yas_each_next(each)) {
            auto const &idx = yas_each_index(each);
            auto const &cell_regions = this->_time_strings->actual_cell_regions();

            if (idx < cell_regions.size()) {
                auto const &cell_region = cell_regions.at(idx);

                if (region.has_value()) {
                    region = region.value().combined(cell_region);
                } else {
                    region = cell_region;
                }
            }
        }

        if (region.has_value()) {
            auto const &region_value = region.value();
            this->_editing_frame->node()->set_position(region_value.origin);
            this->_editing_frame->node()->set_scale(region_value.size);

            this->_editing_frame->node()->set_is_enabled(true);

            return;
        }
    }

    this->_editing_frame->node()->set_is_enabled(false);
}

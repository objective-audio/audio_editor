//
//  ae_ui_track.mm
//

#include "ae_ui_track.h"
#include <audio_editor_core/ae_track_presenter.h>
#include <audio_editor_core/ae_ui_track_constants.h>
#include <cpp_utils/yas_fast_each.h>

using namespace yas;
using namespace yas::ae;

ui_track::ui_track(std::shared_ptr<ui::standard> const &standard, std::shared_ptr<track_presenter> const &presenter)
    : _presenter(presenter),
      _node(ui::node::make_shared()),
      _rect_plane(ui::rect_plane::make_shared(ui_track_constants::max_plane_count)) {
    this->_rect_plane->node()->mesh()->set_use_mesh_color(true);

    auto const &data = this->_rect_plane->data();
    data->set_rect_count(0);

    auto each = make_fast_each(data->max_rect_count());
    while (yas_each_next(each)) {
        auto const &idx = yas_each_index(each);
        float const hue = float(idx % 6) / 6.0f;
        auto const color = ui::hsb_color(hue, 1.0f, 1.0f);
        data->set_rect_color(color, 1.0f, idx);
    }

    presenter
        ->observe([this](auto const &) {
            auto const file_info = this->_presenter->file_info();
            if (!file_info.has_value()) {
                return;
            }

            auto const &sample_rate = file_info.value().sample_rate;
            auto const h_rate = ui_track_constants::width_per_sec / float(sample_rate);
            auto const &modules = this->_presenter->modules();
            auto const &data = this->_rect_plane->data();
            std::size_t idx = 0;
            for (auto const &pair : modules) {
                float const x = float(pair.second.range.frame) * h_rate;
                float const width = float(pair.second.range.length) * h_rate;
                data->set_rect_position({ui::point{x, 0.0f}, ui::size{width, ui_track_constants::height}}, idx);
                ++idx;
            }
            data->set_rect_count(modules.size());
        })
        .sync()
        ->add_to(this->_pool);

    this->_node->add_sub_node(this->_rect_plane->node());

    standard->renderer()
        ->observe_will_render([this](auto const &) {
            auto const time = this->_presenter->current_time();
            float const x = -time * ui_track_constants::width_per_sec;
            this->_rect_plane->node()->set_position(ui::point{x, 0.0f});
        })
        .end()
        ->add_to(this->_pool);
}

std::shared_ptr<ui::node> const ui_track::node() const {
    return this->_node;
}

std::shared_ptr<ui_track> ui_track::make_shared(std::shared_ptr<ui::standard> const &standard,
                                                std::string const &project_id) {
    auto const presenter = track_presenter::make_shared(project_id);
    return std::shared_ptr<ui_track>(new ui_track{standard, presenter});
}

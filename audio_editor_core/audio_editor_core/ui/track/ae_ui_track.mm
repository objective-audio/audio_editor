//
//  ae_ui_track.mm
//

#include "ae_ui_track.h"
#include <audio_editor_core/ae_track_presenter.h>
#include <audio_editor_core/ae_ui_marker_plane.h>
#include <audio_editor_core/ae_ui_module_plane.h>
#include <audio_editor_core/ae_ui_track_constants.h>
#include <cpp_utils/yas_fast_each.h>

using namespace yas;
using namespace yas::ae;

ui_track::ui_track(std::shared_ptr<ui::standard> const &standard, std::shared_ptr<track_presenter> const &presenter)
    : _presenter(presenter),
      _root_node(ui::node::make_shared()),
      _time_node(ui::node::make_shared()),
      _module_plane(ui_module_plane::make_shared(ui_track_constants::max_plane_count)),
      _marker_plane(ui_marker_plane::make_shared(10)) {
    this->_root_node->add_sub_node(this->_time_node);
    this->_time_node->add_sub_node(this->_module_plane->node());
    this->_time_node->add_sub_node(this->_marker_plane->node());

    ui::size const scale{ui_track_constants::width_per_sec, ui_track_constants::height};
    this->_module_plane->set_scale(scale);
    this->_marker_plane->set_scale(scale);

    presenter
        ->observe_modules([this] {
            auto const file_info = this->_presenter->file_info();
            if (!file_info.has_value()) {
                return;
            }

            auto const &sample_rate = file_info.value().sample_rate;
            auto const &modules = this->_presenter->modules();

            std::vector<ui_module_plane::element> elements;
            elements.reserve(modules.size());

            for (auto const &pair : modules) {
                float const x = float(pair.second.range.frame) / float(sample_rate);
                float const width = float(pair.second.range.length) / float(sample_rate);
                elements.emplace_back(ui_module_plane::element{x, width});
            }

            this->_module_plane->set_elements(elements);
        })
        .sync()
        ->add_to(this->_pool);

    presenter
        ->observe_markers([this] {
            auto const file_info = this->_presenter->file_info();
            if (!file_info.has_value()) {
                return;
            }

            auto const &sample_rate = file_info.value().sample_rate;
            auto const &markers = this->_presenter->markers();

            std::vector<ui_marker_plane::element> elements;
            elements.reserve(markers.size());

            for (auto const &pair : markers) {
                float const position = float(pair.first) / float(sample_rate);
                elements.emplace_back(ui_marker_plane::element{position});
            }

            this->_marker_plane->set_elements(std::move(elements));
        })
        .sync()
        ->add_to(this->_pool);

    standard->renderer()
        ->observe_will_render([this](auto const &) {
            auto const time = this->_presenter->current_time();
            float const x = -time * ui_track_constants::width_per_sec;
            this->_time_node->set_position(ui::point{x, 0.0f});
        })
        .end()
        ->add_to(this->_pool);
}

std::shared_ptr<ui::node> const ui_track::node() const {
    return this->_root_node;
}

std::shared_ptr<ui_track> ui_track::make_shared(std::shared_ptr<ui::standard> const &standard,
                                                std::string const &project_id) {
    auto const presenter = track_presenter::make_shared(project_id);
    return std::shared_ptr<ui_track>(new ui_track{standard, presenter});
}

//
//  ae_ui_tracks.mm
//

#include "ae_ui_tracks.hpp"
#include <audio_editor_core/ae_color.h>
#include <audio_editor_core/ae_ui_hierarchy.h>
#include <audio_editor_core/ae_modifiers_holder.hpp>
#include <audio_editor_core/ae_tracks_controller.hpp>
#include <audio_editor_core/ae_tracks_presenter.hpp>
#include <audio_editor_core/ae_ui_atlas.hpp>
#include <audio_editor_core/ae_ui_mesh_utils.hpp>

using namespace yas;
using namespace yas::ae;

namespace yas::ae::ui_tracks_constants {
static std::size_t constexpr reserving_interval = 64;
static float constexpr square_width = 20.0f;
static float constexpr square_height_rate = 0.95f;
}

std::shared_ptr<ui_tracks> ui_tracks::make_shared(project_lifetime_id const &project_lifetime_id, ui::node *node) {
    auto const presenter = tracks_presenter::make_shared(project_lifetime_id);
    auto const controller = tracks_controller::make_shared(project_lifetime_id);
    auto const &app_lifetime = hierarchy::app_lifetime();
    auto const &resource_lifetime = ui_hierarchy::resource_lifetime_for_project_lifetime_id(project_lifetime_id);
    return std::make_shared<ui_tracks>(project_lifetime_id, node, presenter, controller, resource_lifetime->standard,
                                       app_lifetime->color.get(), resource_lifetime->modifiers_holder.get(),
                                       resource_lifetime->atlas.get());
}

ui_tracks::ui_tracks(project_lifetime_id const &project_lifetime_id, ui::node *node,
                     std::shared_ptr<tracks_presenter> const &presenter,
                     std::shared_ptr<tracks_controller> const &controller,
                     std::shared_ptr<ui::standard> const &standard, ae::color *color,
                     modifiers_holder *modifiers_holder, ui_atlas const *atlas)
    : _project_lifetime_id(project_lifetime_id),
      _node(node),
      _presenter(presenter),
      _controller(controller),
      _color(color),
      _modifiers_holder(modifiers_holder),
      _atlas(atlas),
      _left_guide(standard->view_look()->view_layout_guide()->left()),
      _vertex_datas(std::make_unique<std::vector<std::shared_ptr<ui::dynamic_mesh_vertex_data>>>()),
      _fill_mesh_container(
          ui_mesh_utils::make_fill_container(this->_vertex_datas.get(), ui_tracks_constants::reserving_interval, true)),
      _touch_tracker(ui::touch_tracker::make_shared(standard, this->_fill_mesh_container->node)) {
    node->add_sub_node(this->_fill_mesh_container->node);

    presenter
        ->observe_contents([this](track_content_pool_event const &event) {
            switch (event.type) {
                case track_content_pool_event_type::fetched:
                case track_content_pool_event_type::replaced:
                    this->_replace_data();
                    break;
                case track_content_pool_event_type::updated:
                    this->_update_data(event.inserted, event.replaced, event.erased);
                    break;
            }
        })
        .sync()
        ->add_to(this->_pool);

    standard->renderer()
        ->observe_will_render([this](auto const &) { this->_presenter->update_if_needed(); })
        .end()
        ->add_to(this->_pool);

    standard->view_look()
        ->observe_appearance([this](auto const &) { this->_update_colors(); })
        .sync()
        ->add_to(this->_pool);

    atlas
        ->observe_white_filled_tex_coords(
            [vertex_datas = this->_vertex_datas.get()](ui::uint_region const &tex_coords) {
                for (auto const &vertex_data : *vertex_datas) {
                    vertex_data->write([&tex_coords](std::vector<ui::vertex2d_t> &vertices) {
                        for (auto &vertex : vertices) {
                            vertex.tex_coord = to_float2(tex_coords.origin);
                        }
                    });
                }
            })
        .sync()
        ->add_to(this->_pool);

    this->_touch_tracker
        ->observe([this](ui::touch_tracker::context const &context) {
            if (context.touch_event.touch_id == ui::touch_id::mouse_left()) {
                auto const &modifiers = this->_modifiers_holder->modifiers();

                switch (context.phase) {
                    case ui::touch_tracker_phase::began:
                        this->_began_collider_idx = context.collider_idx;
                        if (modifiers.empty()) {
                            this->_controller->deselect_all();
                            this->_controller->begin_range_selection(context.touch_event.position);
                        } else if (modifiers.size() == 1 && modifiers.contains(ae::modifier::shift)) {
                            this->_controller->begin_range_selection(context.touch_event.position);
                        }
                        break;
                    case ui::touch_tracker_phase::ended:
                        if (this->_began_collider_idx == context.collider_idx) {
                            if (modifiers.contains(ae::modifier::command)) {
                                this->_controller->toggle_selection(context.collider_idx);
                            }
                        }
                        this->_began_collider_idx = std::nullopt;
                        break;
                    case ui::touch_tracker_phase::canceled:
                    case ui::touch_tracker_phase::leaved:
                        this->_began_collider_idx = std::nullopt;
                        break;
                    case ui::touch_tracker_phase::moved:
                    case ui::touch_tracker_phase::entered:
                        break;
                }
            }
        })
        .end()
        ->add_to(this->_pool);

    this->_presenter
        ->observe_range([this](range_selection const &selection) {
            switch (selection.phase) {
                case range_selection_phase::began:
                    this->_controller->begin_selection();
                case range_selection_phase::moved: {
                    auto const &range = selection.range;
                    if (range.has_value()) {
                        auto const region_value = range.value().region();
                        std::vector<std::size_t> hit_indices;

                        auto const &colliders = this->_fill_mesh_container->node->colliders();
                        auto each = make_fast_each(colliders.size());
                        while (yas_each_next(each)) {
                            auto const &idx = yas_each_index(each);
                            auto const &collider = colliders.at(idx);
                            if (collider->hit_test(region_value)) {
                                hit_indices.emplace_back(idx);
                            }
                        }

                        this->_controller->select(hit_indices);
                    }
                } break;
                case range_selection_phase::ended:
                    this->_controller->end_selection();
                    break;
            }
        })
        .sync()
        ->add_to(this->_pool);

    this->_fill_mesh_container->node->attach_x_layout_guide(*this->_left_guide);
}

void ui_tracks::set_scale(ui::size const &scale) {
    this->_scale = scale;
    this->_fill_mesh_container->node->set_scale({.width = 1.0f, .height = scale.height});
}

void ui_tracks::_replace_data() {
    auto const &contents = this->_presenter->contents();

    this->_set_rect_count(contents.size());

    this->_fill_mesh_container->write_vertex_elements(
        [this, &contents](index_range const range, vertex2d_rect *vertex_rects) {
            if (contents.size() <= range.index) {
                return;
            }

            auto const &colliders = this->_fill_mesh_container->node->colliders();
            auto const &filled_tex_coords = this->_atlas->white_filled_tex_coords();

            auto const process_length = std::min(range.length, contents.size() - range.index);
            auto each = make_fast_each(process_length);
            while (yas_each_next(each)) {
                auto const &vertex_idx = yas_each_index(each);
                auto const content_idx = range.index + vertex_idx;

                auto const &content = contents.at(content_idx);
                auto const &collider = colliders.at(content_idx);
                auto &rect = vertex_rects[vertex_idx];

                if (content.has_value()) {
                    auto const &value = content.value();
                    ui::region const region{
                        .origin = {.x = 0.0f, .y = value.track() - (ui_tracks_constants::square_height_rate * 0.5f)},
                        .size = {.width = ui_tracks_constants::square_width,
                                 .height = ui_tracks_constants::square_height_rate}};
                    rect.set_position(region);
                    rect.set_tex_coord(filled_tex_coords);

                    collider->set_shape(ui::shape::make_shared({.rect = region}));
                    collider->set_enabled(true);
                } else {
                    rect.set_position(ui::region::zero());

                    collider->set_enabled(false);
                    collider->set_shape(nullptr);
                }
            }
        });

    this->_update_colors();
}

// こことは別に_update_dataで部分的に色を更新しているので、変更する際は注意
void ui_tracks::_update_colors() {
    auto const &contents = this->_presenter->contents();

    this->_fill_mesh_container->write_vertex_elements([this, &contents](index_range const range,
                                                                        vertex2d_rect *vertex_rects) {
        if (contents.size() <= range.index) {
            return;
        }

        auto const normal_square_color = this->_color->track_square().v;
        auto const selected_square_color = this->_color->selected_track_square().v;
        auto const process_length = std::min(range.length, contents.size() - range.index);

        auto each = make_fast_each(process_length);
        while (yas_each_next(each)) {
            auto const &vertex_idx = yas_each_index(each);
            auto const content_idx = range.index + vertex_idx;
            auto const &content = contents.at(content_idx);

            if (content.has_value()) {
                auto const &square_color = content.value().is_selected ? selected_square_color : normal_square_color;
                vertex_rects[vertex_idx].set_color(square_color);
            }
        }
    });
}

void ui_tracks::_update_data(std::set<std::size_t> const &inserted, std::set<std::size_t> const &replaced,
                             std::map<std::size_t, track_content> const &erased) {
    auto const &contents = this->_presenter->contents();

    this->_set_rect_count(contents.size());

    this->_fill_mesh_container->write_vertex_elements(
        [&contents, &erased, &inserted, &replaced, this](index_range const range, vertex2d_rect *vertex_rects) {
            auto const &colliders = this->_fill_mesh_container->node->colliders();

            for (auto const &pair : erased) {
                auto const &content_idx = pair.first;
                if (range.contains(content_idx)) {
                    auto const vertex_idx = content_idx - range.index;
                    vertex_rects[vertex_idx].set_position(ui::region::zero());

                    auto const &collider = colliders.at(content_idx);
                    collider->set_enabled(false);
                    collider->set_shape(nullptr);
                }
            }

            auto const &color = this->_color;
            auto const normal_square_color = color->track_square().v;
            auto const selected_square_color = color->selected_track_square().v;
            auto const &filled_tex_coords = this->_atlas->white_filled_tex_coords();

            for (auto const &content_idx : inserted) {
                if (range.contains(content_idx)) {
                    auto const vertex_idx = content_idx - range.index;
                    auto const &value = contents.at(content_idx).value();
                    ui::region const region{
                        .origin = {.x = 0.0f, .y = value.track() - (ui_tracks_constants::square_height_rate * 0.5f)},
                        .size = {.width = ui_tracks_constants::square_width,
                                 .height = ui_tracks_constants::square_height_rate}};

                    auto &rect = vertex_rects[vertex_idx];
                    rect.set_position(region);
                    rect.set_tex_coord(filled_tex_coords);

                    auto const &square_color = value.is_selected ? selected_square_color : normal_square_color;
                    rect.set_color(square_color);

                    auto const &collider = colliders.at(content_idx);
                    collider->set_shape(ui::shape::make_shared({.rect = region}));
                    collider->set_enabled(true);
                }
            }

            for (auto const &content_idx : replaced) {
                if (range.contains(content_idx)) {
                    auto const vertex_idx = content_idx - range.index;
                    auto const &value = contents.at(content_idx).value();
                    auto const &square_color = value.is_selected ? selected_square_color : normal_square_color;
                    vertex_rects[vertex_idx].set_color(square_color);
                }
            }
        });
}

void ui_tracks::_set_rect_count(std::size_t const rect_count) {
    this->_fill_mesh_container->set_element_count(rect_count);

    auto const reserving_count = common_utils::reserving_count(rect_count, ui_tracks_constants::reserving_interval);

    auto const colliders_size = this->_fill_mesh_container->node->colliders().size();
    if (colliders_size < reserving_count) {
        auto each = make_fast_each(reserving_count - colliders_size);
        while (yas_each_next(each)) {
            auto collider = ui::collider::make_shared();
            collider->set_enabled(false);
            this->_fill_mesh_container->node->push_back_collider(std::move(collider));
        }
    }
}

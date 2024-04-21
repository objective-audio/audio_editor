//
//  ae_ui_markers.mm
//

#include "ae_ui_markers.h"
#include <ae-core/app/features/ae_color.h>
#include <ae-core/global/ae_ui_hierarchy.h>
#include <ae-core/global/utils/ae_common_utils.h>
#include <ae-core/ui_base/features/common/ae_ui_mesh_data.h>
#include <ae-core/ui_base/features/markers/ae_ui_marker_name.h>
#include <ae-core/ui_base/features/markers/presenter/ae_markers_presenter.h>
#include <ae-core/ui_base/utils/ae_ui_layout_utils.h>
#include <ae-core/ui_base/features/common/ae_ui_atlas.hpp>
#include <ae-core/ui_base/features/markers/controller/ae_markers_controller.hpp>
#include <ae-core/ui_base/utils/ae_ui_mesh_utils.hpp>
#include <ae-core/ui_resource/features/ae_modifiers_holder.hpp>

using namespace yas;
using namespace yas::ae;

namespace yas::ae::ui_markers_constants {
static std::size_t const reserving_interval = 16;
}

std::shared_ptr<ui_markers> ui_markers::make_shared(project_lifetime_id const &project_lifetime_id,
                                                    std::shared_ptr<ui::node> const &node) {
    auto const &app_lifetime = hierarchy::app_lifetime();
    auto const &project_editing_lifetime = hierarchy::project_editing_lifetime_for_id(project_lifetime_id);
    auto const &resource_lifetime = ui_hierarchy::resource_lifetime_for_project_lifetime_id(project_lifetime_id);

    auto const presenter =
        markers_presenter::make_shared(project_lifetime_id, project_editing_lifetime->marker_content_pool);
    auto const controller = markers_controller::make_shared(project_lifetime_id);
    return std::make_shared<ui_markers>(project_lifetime_id, presenter, controller, resource_lifetime->standard,
                                        resource_lifetime->normal_font_atlas, node.get(),
                                        resource_lifetime->modifiers_holder.get(), resource_lifetime->atlas.get(),
                                        app_lifetime->color.get());
}

ui_markers::ui_markers(project_lifetime_id const &project_lifetime_id,
                       std::shared_ptr<markers_presenter> const &presenter,
                       std::shared_ptr<markers_controller> const &controller,
                       std::shared_ptr<ui::standard> const &standard, std::shared_ptr<ui::font_atlas> const &font_atlas,
                       ui::node *node, modifiers_holder *modifiers_holder, ui_atlas const *atlas,
                       ae::color const *color)
    : _project_lifetime_id(project_lifetime_id),
      _node(node),
      _names_root_node(ui::node::make_shared()),
      _presenter(presenter),
      _controller(controller),
      _color(color),
      _atlas(atlas),
      _top_guide(standard->view_look()->view_layout_guide()->top()),
      _square_vertex_datas(std::make_unique<std::vector<std::shared_ptr<ui::dynamic_mesh_vertex_data>>>()),
      _square_fill_mesh_container(ui_mesh_utils::make_fill_container(this->_square_vertex_datas.get(),
                                                                     ui_markers_constants::reserving_interval, true)),
      _line_vertex_datas(std::make_unique<std::vector<std::shared_ptr<ui::dynamic_mesh_vertex_data>>>()),
      _line_mesh_container(ui_mesh_utils::make_fill_container(this->_line_vertex_datas.get(),
                                                              ui_markers_constants::reserving_interval, false)),
      _touch_tracker(ui::touch_tracker::make_shared(standard, this->_square_fill_mesh_container->node)),
      _multiple_touch(ui::multiple_touch::make_shared()),
      _modifiers_holder(modifiers_holder) {
    node->add_sub_node(this->_square_fill_mesh_container->node);
    node->add_sub_node(this->_line_mesh_container->node);
    node->add_sub_node(this->_names_root_node);

    this->_presenter
        ->observe_contents([this](marker_content_pool_event const &event) {
            switch (event.type) {
                case marker_content_pool_event_type::fetched:
                case marker_content_pool_event_type::replaced:
                    this->_replace_data();
                    break;
                case marker_content_pool_event_type::updated:
                    this->_update_data(event.inserted, event.replaced, event.erased);
                    break;
            }
        })
        .sync()
        ->add_to(this->_pool);

    standard->renderer()
        ->observe_will_render([this](auto const &) {
            this->_presenter->update_if_needed();
            this->_line_mesh_container->reduce_if_needed();
            this->_square_fill_mesh_container->reduce_if_needed();
        })
        .end()
        ->add_to(this->_pool);

    standard->view_look()
        ->observe_appearance([this](auto const &) { this->_update_colors(); })
        .sync()
        ->add_to(this->_pool);

    font_atlas->observe_rects_updated(1, [this](auto const &) { this->_update_square_regions(); })
        .end()
        ->add_to(this->_pool);

    this->_square_fill_mesh_container->node->attach_y_layout_guide(*this->_top_guide);
    this->_names_root_node->attach_y_layout_guide(*this->_top_guide);

    standard->view_look()
        ->view_layout_guide()
        ->observe([this](ui::region const &region) {
            auto const &node = this->_line_mesh_container->node;
            node->set_y(-region.size.height * 0.5f);
            node->set_scale({.width = 1.0f, .height = region.size.height});
        })
        .sync()
        ->add_to(this->_pool);

    atlas
        ->observe_white_filled_tex_coords(
            [square_vertex_datas = this->_square_vertex_datas.get(),
             line_vertex_data = this->_line_vertex_datas.get()](ui::uint_region const &tex_coords) {
                for (auto const &vertex_data : *square_vertex_datas) {
                    vertex_data->write([&tex_coords](std::vector<ui::vertex2d_t> &vertices) {
                        for (auto &vertex : vertices) {
                            vertex.tex_coord = to_float2(tex_coords.origin);
                        }
                    });
                }

                for (auto const &vertex_data : *line_vertex_data) {
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
                                if (auto const &content = this->_presenter->contents().at(context.collider_idx)) {
                                    this->_controller->toggle_selection(content.value().marker_index());
                                }
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

                this->_multiple_touch->handle_event(context.phase, context.collider_idx);
            }
        })
        .end()
        ->add_to(this->_pool);

    this->_multiple_touch
        ->observe([this](std::uintptr_t const &collider_idx) {
            if (auto const &content = this->_presenter->contents().at(collider_idx); content.has_value()) {
                this->_controller->begin_renaming(content.value().marker_index());
            }
        })
        .end()
        ->add_to(this->_pool);

    presenter
        ->observe_range([this](range_selection const &selection) {
            switch (selection.phase) {
                case range_selection_phase::began:
                    this->_controller->begin_selection();
                case range_selection_phase::moved: {
                    auto const &range = selection.range;
                    if (range.has_value()) {
                        auto const rect_value = range.value().region();
                        std::set<marker_index> hit_indices;

                        auto const &colliders = this->_square_fill_mesh_container->node->colliders();
                        auto each = make_fast_each(colliders.size());
                        while (yas_each_next(each)) {
                            auto const &idx = yas_each_index(each);
                            auto const &collider = colliders.at(idx);
                            if (collider->hit_test(rect_value)) {
                                if (auto const &marker_index = this->_presenter->marker_index_at(idx);
                                    marker_index.has_value()) {
                                    hit_indices.emplace(marker_index.value());
                                }
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
}

void ui_markers::_replace_data() {
    auto const &contents = this->_presenter->contents();
    auto const &names = this->_names;

    this->_set_rect_count(contents.size());

    this->_square_fill_mesh_container->write_vertex_elements(
        [this, &contents, &names](index_range const range, vertex2d_rect *vertex_rects) {
            if (contents.size() <= range.index) {
                return;
            }

            auto const &colliders = this->_square_fill_mesh_container->node->colliders();
            auto const &filled_tex_coords = this->_atlas->white_filled_tex_coords();

            auto const process_length = std::min(range.length, contents.size() - range.index);
            auto each = make_fast_each(process_length);
            while (yas_each_next(each)) {
                auto const &vertex_idx = yas_each_index(each);
                auto const content_idx = range.index + vertex_idx;

                auto const &content = contents.at(content_idx);
                auto const &name = names.at(content_idx);
                auto const &collider = colliders.at(content_idx);
                auto &rect = vertex_rects[vertex_idx];

                if (content.has_value()) {
                    name->set_content(content.value());

                    rect.set_tex_coord(filled_tex_coords);

                    collider->set_enabled(true);
                } else {
                    name->reset_content();

                    rect.set_position(ui::region::zero());

                    collider->set_enabled(false);
                    collider->set_shape(nullptr);
                }
            }
        });

    this->_line_mesh_container->write_vertex_elements(
        [this, &contents](index_range const range, vertex2d_rect *vertex_rects) {
            if (contents.size() <= range.index) {
                return;
            }

            auto const &filled_tex_coords = this->_atlas->white_filled_tex_coords();

            auto const process_length = std::min(range.length, contents.size() - range.index);
            auto each = make_fast_each(process_length);
            while (yas_each_next(each)) {
                auto const &vertex_idx = yas_each_index(each);
                auto const content_idx = range.index + vertex_idx;

                auto const &content = contents.at(content_idx);
                auto &rect = vertex_rects[vertex_idx];

                if (content.has_value()) {
                    rect.set_position({.origin = {content.value().x(), 0.0f}, .size = {0.5f, 1.0f}});
                    rect.set_tex_coord(filled_tex_coords);
                } else {
                    rect.set_position(ui::region::zero());
                }
            }
        });

    this->_update_square_regions();
    this->_update_colors();
}

// こことは別に_update_dataで部分的に色を更新しているので、変更する際は注意
void ui_markers::_update_colors() {
    auto const &contents = this->_presenter->contents();
    auto const normal_bg_color = this->_color->marker_square().v;
    auto const selected_bg_color = this->_color->selected_marker_square().v;
    auto const normal_color = this->_color->marker_text();
    auto const selected_color = this->_color->selected_marker_text();

    this->_square_fill_mesh_container->write_vertex_elements(
        [this, &contents, &normal_bg_color, &selected_bg_color](index_range const range, vertex2d_rect *vertex_rects) {
            if (contents.size() <= range.index) {
                return;
            }

            auto const process_length = std::min(range.length, contents.size() - range.index);

            auto each = make_fast_each(process_length);
            while (yas_each_next(each)) {
                auto const &vertex_idx = yas_each_index(each);
                auto const content_idx = range.index + vertex_idx;
                auto const &content = contents.at(content_idx);

                if (content.has_value()) {
                    auto const &bg_color = content.value().is_selected ? selected_bg_color : normal_bg_color;
                    vertex_rects[vertex_idx].set_color(bg_color);
                }
            }
        });

    auto each = make_fast_each(this->_names.size());
    while (yas_each_next(each)) {
        auto const &idx = yas_each_index(each);
        auto const &name = this->_names.at(idx);
        name->update_color(selected_color, normal_color);
    }

    this->_line_mesh_container->node->set_color(this->_color->marker_line());
}

void ui_markers::_update_data(std::set<std::size_t> const &inserted, std::set<std::size_t> const &replaced,
                              std::map<std::size_t, marker_content> const &erased) {
    auto const &contents = this->_presenter->contents();
    auto const &names = this->_names;
    auto const contents_size = contents.size();
    auto const &color = this->_color;
    auto const normal_bg_color = color->marker_square().v;
    auto const selected_bg_color = color->selected_marker_square().v;
    auto const normal_name_color = color->marker_text();
    auto const selected_name_color = color->selected_marker_text();

    this->_set_rect_count(contents_size);

    this->_square_fill_mesh_container->write_vertex_elements(
        [&contents, &names, &erased, &inserted, &replaced, this, &normal_bg_color, &selected_bg_color,
         &normal_name_color, &selected_name_color](index_range const range, vertex2d_rect *vertex_rects) {
            auto const &colliders = this->_square_fill_mesh_container->node->colliders();

            for (auto const &pair : erased) {
                auto const &content_idx = pair.first;
                if (range.contains(content_idx)) {
                    auto const vertex_idx = content_idx - range.index;
                    vertex_rects[vertex_idx].set_position(ui::region::zero());

                    auto const &collider = colliders.at(content_idx);
                    collider->set_enabled(false);
                    collider->set_shape(nullptr);

                    names.at(content_idx)->reset_content();
                }
            }

            auto const &filled_tex_coords = this->_atlas->white_filled_tex_coords();

            for (auto const &content_idx : inserted) {
                if (range.contains(content_idx)) {
                    auto const vertex_idx = content_idx - range.index;
                    auto const &content = contents.at(content_idx).value();
                    auto const &name = names.at(content_idx);

                    name->set_content(content);
                    this->_update_square_region(content_idx);
                    name->update_color(selected_name_color, normal_name_color);

                    auto &rect = vertex_rects[vertex_idx];
                    rect.set_tex_coord(filled_tex_coords);

                    auto const &bg_color = content.is_selected ? selected_bg_color : normal_bg_color;
                    rect.set_color(bg_color);

                    auto const &collider = colliders.at(content_idx);
                    collider->set_enabled(true);
                }
            }

            for (auto const &content_idx : replaced) {
                if (range.contains(content_idx)) {
                    auto const vertex_idx = content_idx - range.index;
                    auto const &content = contents.at(content_idx).value();
                    auto const &name = names.at(content_idx);

                    name->update_content(content);
                    this->_update_square_region(content_idx);
                    name->update_color(selected_name_color, normal_name_color);

                    auto const &bg_color = content.is_selected ? selected_bg_color : normal_bg_color;
                    vertex_rects[vertex_idx].set_color(bg_color);
                }
            }
        });

    this->_line_mesh_container->write_vertex_elements(
        [&contents, &erased, &inserted, &replaced, this](index_range const range, vertex2d_rect *vertex_rects) {
            for (auto const &pair : erased) {
                auto const &content_idx = pair.first;
                if (range.contains(content_idx)) {
                    auto const vertex_idx = content_idx - range.index;
                    vertex_rects[vertex_idx].set_position(ui::region::zero());
                }
            }

            auto const &filled_tex_coords = this->_atlas->white_filled_tex_coords();

            for (auto const &content_idx : inserted) {
                if (range.contains(content_idx)) {
                    auto const vertex_idx = content_idx - range.index;
                    auto const &content = contents.at(content_idx);

                    auto &rect = vertex_rects[vertex_idx];
                    rect.set_position({.origin = {content.value().x(), 0.0f}, .size = {0.5f, 1.0f}});
                    rect.set_tex_coord(filled_tex_coords);
                }
            }
        });
}

void ui_markers::_set_rect_count(std::size_t const rect_count) {
    auto const prev_names_count = this->_names.size();
    auto const reserving_count = common_utils::reserving_count(rect_count, ui_markers_constants::reserving_interval);

    this->_square_fill_mesh_container->set_element_count(rect_count);
    this->_line_mesh_container->set_element_count(rect_count);

    if (prev_names_count < rect_count) {
        this->_names.reserve(reserving_count);

        auto each = make_fast_each(rect_count - prev_names_count);
        while (yas_each_next(each)) {
            this->_names.emplace_back(
                ui_marker_name::make_unique(this->_project_lifetime_id, this->_names_root_node.get()));
        }
    } else if (rect_count < prev_names_count) {
        auto each = make_fast_each(prev_names_count - rect_count);
        while (yas_each_next(each)) {
            auto const idx = prev_names_count - 1 - yas_each_index(each);
            this->_names.at(idx)->finalize();
        }
        this->_names.resize(rect_count);
    }

    auto const colliders_size = this->_square_fill_mesh_container->node->colliders().size();
    if (colliders_size < reserving_count) {
        auto each = make_fast_each(reserving_count - colliders_size);
        while (yas_each_next(each)) {
            auto collider = ui::collider::make_shared();
            collider->set_enabled(false);
            this->_square_fill_mesh_container->node->push_back_collider(std::move(collider));
        }
    }
}

void ui_markers::_update_square_region(std::size_t const content_idx) {
    this->_square_fill_mesh_container->write_vertex_element(content_idx, [this, &content_idx](vertex2d_rect *rect) {
        auto const &colliders = this->_square_fill_mesh_container->node->colliders();
        auto const square_region = this->_names.at(content_idx)->square_region();

        rect->set_position(square_region);

        auto const &collider = colliders.at(content_idx);
        collider->set_shape(ui::shape::make_shared({.rect = square_region}));
    });
}

void ui_markers::_update_square_regions() {
    auto const &contents = this->_presenter->contents();
    auto const &colliders = this->_square_fill_mesh_container->node->colliders();
    auto const &names = this->_names;

    this->_square_fill_mesh_container->write_vertex_elements(
        [&contents, &colliders, &names](index_range const range, vertex2d_rect *vertex_rects) {
            if (contents.size() <= range.index) {
                return;
            }

            auto const process_length = std::min(range.length, contents.size() - range.index);
            auto each = make_fast_each(process_length);
            while (yas_each_next(each)) {
                auto const &vertex_idx = yas_each_index(each);
                auto const content_idx = range.index + vertex_idx;
                auto const square_region = names.at(content_idx)->square_region();

                auto &rect = vertex_rects[vertex_idx];
                rect.set_position(square_region);

                auto const &collider = colliders.at(content_idx);
                collider->set_shape(ui::shape::make_shared({.rect = square_region}));
            }
        });
}

//
//  ae_ui_dynamic_mesh_container.hpp
//

#pragma once

#include <cpp_utils/yas_system_time_provider.h>
#include <ui/yas_ui_umbrella.h>

namespace yas::ae {
struct dynamic_mesh_content {
    std::shared_ptr<ui::dynamic_mesh_vertex_data> const vertex_data;
    std::shared_ptr<ui::dynamic_mesh_index_data> const index_data;
    std::shared_ptr<ui::mesh> const mesh;
};

struct dynamic_mesh_reducing {
    std::size_t contents_count;
    time_point_t time_point;
};

template <typename VertexElement, typename IndexElement>
struct dynamic_mesh_container {
    using make_content_f =
        std::function<std::unique_ptr<dynamic_mesh_content>(std::size_t const idx, std::size_t const element_count)>;
    using duration_t = std::chrono::duration<double>;

    std::shared_ptr<ui::node> const node = ui::node::make_shared();

    dynamic_mesh_container(
        std::size_t const interval, make_content_f &&,
        std::shared_ptr<system_time_providable> const &provider = system_time_provider::make_shared(),
        duration_t const reducing_duration = duration_t{10.0});

    [[nodiscard]] std::size_t reserved_element_count() const;
    [[nodiscard]] std::size_t element_count() const;
    void set_element_count(std::size_t const);
    void write_vertex_elements(std::function<void(index_range const, VertexElement *)> const &);
    void write_vertex_element(std::size_t const idx, std::function<void(VertexElement *)> const &);

    void reduce_if_needed();

   private:
    std::size_t _element_count;
    std::size_t const _interval;
    std::vector<std::unique_ptr<dynamic_mesh_content>> _contents;
    make_content_f const _make_content;
    std::shared_ptr<system_time_providable> const _system_time_provider;
    duration_t const _reducing_duration;

    std::vector<dynamic_mesh_reducing> _reducings;

    void _remake_contents_if_needed(std::size_t const elements_count);
    [[nodiscard]] std::size_t _available_contents_count() const;
};
}  // namespace yas::ae

#include <audio_editor_core/ae_ui_dynamic_mesh_container_private.h>

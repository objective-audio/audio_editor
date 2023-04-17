//
//  ae_ui_dynamic_mesh_container.hpp
//

#pragma once

#include <ui/yas_ui_umbrella.h>

namespace yas::ae {
struct dynamic_mesh_content {
    std::shared_ptr<ui::dynamic_mesh_vertex_data> const vertex_data;
    std::shared_ptr<ui::dynamic_mesh_index_data> const index_data;
    std::shared_ptr<ui::mesh> const mesh;
};

template <typename VertexElement, typename IndexElement>
struct dynamic_mesh_container {
    using make_content_f =
        std::function<std::unique_ptr<dynamic_mesh_content>(std::size_t const idx, std::size_t const element_count)>;

    std::shared_ptr<ui::node> const node = ui::node::make_shared();

    dynamic_mesh_container(std::size_t const interval, make_content_f &&);

    [[nodiscard]] std::size_t reserved_element_count() const;
    void set_element_count(std::size_t const);
    void write_vertex_elements(std::function<void(index_range const, VertexElement *)> const &);

   private:
    std::size_t const _interval;
    std::vector<std::unique_ptr<dynamic_mesh_content>> _contents;
    make_content_f const _make_content;

    void _remake_contents_if_needed(std::size_t const elements_count);
};
}  // namespace yas::ae

#include <audio_editor_core/ae_ui_dynamic_mesh_container_private.h>

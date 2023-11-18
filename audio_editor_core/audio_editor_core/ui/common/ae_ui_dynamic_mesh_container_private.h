//
//  ae_ui_dynamic_mesh_container_private.h
//

#pragma once

#include <audio_editor_core/ae_common_utils.h>

namespace yas::ae {
template <typename VertexElement, typename IndexElement>
dynamic_mesh_container<VertexElement, IndexElement>::dynamic_mesh_container(std::size_t const interval,
                                                                            make_content_f &&make_content)
    : _element_count(0), _interval(interval), _make_content(std::move(make_content)) {
}

template <typename VertexElement, typename IndexElement>
void dynamic_mesh_container<VertexElement, IndexElement>::set_element_count(std::size_t const element_count) {
    if (this->_element_count == element_count) {
        return;
    }

    this->_element_count = element_count;

    auto const reserving_count = common_utils::reserving_count(element_count, this->_interval);
    this->_remake_contents_if_needed(reserving_count / this->_interval);

    auto const calc_length = [](std::size_t const idx, std::size_t const interval, std::size_t const element_count) {
        auto const top_element_idx = interval * idx;
        if (element_count <= top_element_idx) {
            return std::size_t{0};
        } else {
            auto const mod_element_count = std::min(element_count, top_element_idx + interval) % interval;
            return (mod_element_count == 0) ? interval : mod_element_count;
        }
    };

    auto each = make_fast_each(this->_contents.size());
    while (yas_each_next(each)) {
        auto const &idx = yas_each_index(each);
        auto const length = calc_length(idx, this->_interval, element_count);
        this->_contents.at(idx)->vertex_data->set_count(length * VertexElement::vector_count);
        this->_contents.at(idx)->index_data->set_count(length * IndexElement::vector_count);
    }
}

template <typename VertexElement, typename IndexElement>
std::size_t dynamic_mesh_container<VertexElement, IndexElement>::reserved_element_count() const {
    return this->_interval * this->_contents.size();
}

template <typename VertexElement, typename IndexElement>
std::size_t dynamic_mesh_container<VertexElement, IndexElement>::element_count() const {
    return this->_element_count;
}

template <typename VertexElement, typename IndexElement>
void dynamic_mesh_container<VertexElement, IndexElement>::write_vertex_elements(
    std::function<void(index_range const range, VertexElement *)> const &handler) {
    if (this->_element_count == 0) {
        return;
    }

    std::size_t const element_mod_count = this->_element_count % this->_interval;
    std::size_t const floored_contents_count = this->_element_count / this->_interval;
    std::size_t const contents_count = floored_contents_count + (element_mod_count != 0 ? 1 : 0);

    auto each = make_fast_each(contents_count);
    while (yas_each_next(each)) {
        std::size_t const &content_idx = yas_each_index(each);
        std::size_t const top_element_idx = this->_interval * content_idx;
        std::size_t const length = (content_idx == floored_contents_count) ? element_mod_count : this->_interval;

        this->_contents.at(content_idx)
            ->vertex_data->write([range = index_range{.index = top_element_idx, .length = length},
                                  &handler](std::vector<ui::vertex2d_t> &vertices) {
                handler(range, (VertexElement *)vertices.data());
            });
    }
}

template <typename VertexElement, typename IndexElement>
void dynamic_mesh_container<VertexElement, IndexElement>::write_vertex_element(
    std::size_t const idx, std::function<void(VertexElement *)> const &handler) {
    if (this->_element_count <= idx) {
        return;
    }

    std::size_t const content_idx = (idx == 0) ? 0 : (idx / this->_interval);
    std::size_t const idx_in_content = idx - (content_idx * this->_interval);

    this->_contents.at(content_idx)
        ->vertex_data->write([&handler, idx_in_content](std::vector<ui::vertex2d_t> &vertices) {
            auto *elements = (VertexElement *)vertices.data();
            handler(&elements[idx_in_content]);
        });
}

template <typename VertexElement, typename IndexElement>
void dynamic_mesh_container<VertexElement, IndexElement>::_remake_contents_if_needed(std::size_t const contents_count) {
    // すでに十分に確保されていたら何もせず終了
    if (contents_count <= this->_contents.size()) {
        return;
    }

    auto each = make_fast_each(this->_contents.size(), contents_count);
    while (yas_each_next(each)) {
        auto const &idx = yas_each_index(each);

        this->_contents.emplace_back(this->_make_content(idx, this->_interval));
        auto const &content = this->_contents.at(idx);
        this->node->push_back_mesh(content->mesh);

        content->index_data->write([this](std::vector<ui::index2d_t> &indices) {
            auto *index_elements = (IndexElement *)indices.data();

            auto each = make_fast_each(this->_interval);
            while (yas_each_next(each)) {
                auto const &element_idx = yas_each_index(each);
                uint32_t const element_head_idx = static_cast<uint32_t>(element_idx * VertexElement::vector_count);
                index_elements[element_idx].set_all(element_head_idx);
            }
        });
    }
}
}  // namespace yas::ae

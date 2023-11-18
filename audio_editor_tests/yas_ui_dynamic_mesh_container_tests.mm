#import <XCTest/XCTest.h>
#include <audio_editor_core/ae_ui_dynamic_mesh_container.hpp>

using namespace yas;
using namespace yas::ae;

namespace yas::ae::test_utils {
static std::unique_ptr<dynamic_mesh_container<ui::vertex2d_rect, ui::index2d_rect>> make_container() {
    auto make_content = [](std::size_t const idx, std::size_t const element_count) {
        auto vertex_data = ui::dynamic_mesh_vertex_data::make_shared(element_count * ui::vertex2d_rect::vector_count);
        auto index_data = ui::dynamic_mesh_index_data::make_shared(element_count * ui::index2d_rect::vector_count);
        auto mesh = ui::mesh::make_shared({.primitive_type = ui::primitive_type::triangle, .use_mesh_color = false},
                                          vertex_data, index_data, nullptr);

        return std::unique_ptr<dynamic_mesh_content>(new dynamic_mesh_content{
            .vertex_data = std::move(vertex_data), .index_data = std::move(index_data), .mesh = std::move(mesh)});
    };

    return std::make_unique<dynamic_mesh_container<ui::vertex2d_rect, ui::index2d_rect>>(2, make_content);
}
}

@interface yas_ui_dynamic_mesh_container_tests : XCTestCase

@end

@implementation yas_ui_dynamic_mesh_container_tests

- (void)test_element_count {
    auto const container = test_utils::make_container();

    XCTAssertEqual(container->reserved_element_count(), 0);

    container->set_element_count(1);

    XCTAssertEqual(container->reserved_element_count(), 2);

    container->set_element_count(2);

    XCTAssertEqual(container->reserved_element_count(), 2);

    container->set_element_count(3);

    XCTAssertEqual(container->reserved_element_count(), 4);

    container->set_element_count(1);

    XCTAssertEqual(container->reserved_element_count(), 4);
}

- (void)test_write_elements {
    auto const container = test_utils::make_container();

    std::vector<index_range> called_ranges;

    container->write_vertex_elements(
        [&called_ranges](index_range const range, ui::vertex2d_rect *rects) { called_ranges.emplace_back(range); });

    XCTAssertEqual(called_ranges.size(), 0);

    container->set_element_count(3);

    container->write_vertex_elements(
        [&called_ranges](index_range const range, ui::vertex2d_rect *rects) { called_ranges.emplace_back(range); });

    XCTAssertEqual(called_ranges.size(), 2);
    XCTAssertEqual(called_ranges.at(0), (index_range{.index = 0, .length = 2}));
    XCTAssertEqual(called_ranges.at(1), (index_range{.index = 2, .length = 1}));
}

- (void)test_write_element {
    auto const container = test_utils::make_container();

    std::vector<std::size_t> called_indices;

    container->write_vertex_element(0, [&called_indices](ui::vertex2d_rect *rects) { called_indices.emplace_back(0); });

    XCTAssertEqual(called_indices.size(), 0);

    container->set_element_count(3);

    container->write_vertex_element(0, [&called_indices](ui::vertex2d_rect *rects) { called_indices.emplace_back(0); });

    XCTAssertEqual(called_indices.size(), 1);
    XCTAssertEqual(called_indices.at(0), 0);

    container->write_vertex_element(1, [&called_indices](ui::vertex2d_rect *rects) { called_indices.emplace_back(1); });

    XCTAssertEqual(called_indices.size(), 2);
    XCTAssertEqual(called_indices.at(1), 1);

    container->write_vertex_element(2, [&called_indices](ui::vertex2d_rect *rects) { called_indices.emplace_back(2); });

    XCTAssertEqual(called_indices.size(), 3);
    XCTAssertEqual(called_indices.at(2), 2);

    container->write_vertex_element(3, [&called_indices](ui::vertex2d_rect *rects) { called_indices.emplace_back(3); });

    XCTAssertEqual(called_indices.size(), 3);
}

@end

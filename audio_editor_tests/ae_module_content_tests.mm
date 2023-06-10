//
//  ae_module_content_tests.mm
//

#import <XCTest/XCTest.h>
#include <audio_editor_core/ae_module_content.h>

using namespace yas;
using namespace yas::ae;

@interface ae_module_content_tests : XCTestCase

@end

@implementation ae_module_content_tests

- (void)test_equal {
    auto const id_1 = db::make_temporary_id();
    auto const id_2 = db::make_temporary_id();
    time::range const range_1(0, 1);
    time::range const range_2(0, 2);
    track_index_t const track_1 = 1;
    track_index_t const track_2 = 2;
    bool const is_selected_1 = false;
    bool const is_selected_2 = true;
    uint32_t const sample_rate_1 = 1;
    uint32_t const sample_rate_2 = 2;
    ui::size const scale_1{.width = 1.0f, .height = 1.0f};
    ui::size const scale_2{.width = 2.0f, .height = 2.0f};

    auto make_mesh_elements = [](length_t const length) {
        return std::vector<std::optional<module_content::mesh_element>>{{{.range = {1, length}}}};
    };

    XCTAssertTrue(
        module_content(id_1, range_1, track_1, is_selected_1, sample_rate_1, make_mesh_elements(1), scale_1) ==
        module_content(id_1, range_1, track_1, is_selected_1, sample_rate_1, make_mesh_elements(1), scale_1));
    XCTAssertFalse(
        module_content(id_1, range_1, track_1, is_selected_1, sample_rate_1, make_mesh_elements(1), scale_1) ==
        module_content(id_2, range_1, track_1, is_selected_1, sample_rate_1, make_mesh_elements(1), scale_1));
    XCTAssertFalse(
        module_content(id_1, range_1, track_1, is_selected_1, sample_rate_1, make_mesh_elements(1), scale_1) ==
        module_content(id_1, range_2, track_1, is_selected_1, sample_rate_1, make_mesh_elements(1), scale_1));
    XCTAssertFalse(
        module_content(id_1, range_1, track_1, is_selected_1, sample_rate_1, make_mesh_elements(1), scale_1) ==
        module_content(id_1, range_1, track_2, is_selected_1, sample_rate_1, make_mesh_elements(1), scale_1));
    XCTAssertFalse(
        module_content(id_1, range_1, track_1, is_selected_1, sample_rate_1, make_mesh_elements(1), scale_1) ==
        module_content(id_1, range_1, track_1, is_selected_2, sample_rate_1, make_mesh_elements(1), scale_1));
    XCTAssertFalse(
        module_content(id_1, range_1, track_1, is_selected_1, sample_rate_1, make_mesh_elements(1), scale_1) ==
        module_content(id_1, range_1, track_1, is_selected_1, sample_rate_2, make_mesh_elements(1), scale_1));
    XCTAssertFalse(
        module_content(id_1, range_1, track_1, is_selected_1, sample_rate_1, make_mesh_elements(1), scale_1) ==
        module_content(id_1, range_1, track_1, is_selected_1, sample_rate_1, make_mesh_elements(2), scale_1));
    XCTAssertFalse(
        module_content(id_1, range_1, track_1, is_selected_1, sample_rate_1, make_mesh_elements(1), scale_1) ==
        module_content(id_1, range_1, track_1, is_selected_1, sample_rate_1, make_mesh_elements(1), scale_2));

    XCTAssertFalse(
        module_content(id_1, range_1, track_1, is_selected_1, sample_rate_1, make_mesh_elements(1), scale_1) !=
        module_content(id_1, range_1, track_1, is_selected_1, sample_rate_1, make_mesh_elements(1), scale_1));
    XCTAssertTrue(
        module_content(id_1, range_1, track_1, is_selected_1, sample_rate_1, make_mesh_elements(1), scale_1) !=
        module_content(id_2, range_1, track_1, is_selected_1, sample_rate_1, make_mesh_elements(1), scale_1));
    XCTAssertTrue(
        module_content(id_1, range_1, track_1, is_selected_1, sample_rate_1, make_mesh_elements(1), scale_1) !=
        module_content(id_1, range_2, track_1, is_selected_1, sample_rate_1, make_mesh_elements(1), scale_1));
    XCTAssertTrue(
        module_content(id_1, range_1, track_1, is_selected_1, sample_rate_1, make_mesh_elements(1), scale_1) !=
        module_content(id_1, range_1, track_2, is_selected_1, sample_rate_1, make_mesh_elements(1), scale_1));
    XCTAssertTrue(
        module_content(id_1, range_1, track_1, is_selected_1, sample_rate_1, make_mesh_elements(1), scale_1) !=
        module_content(id_1, range_1, track_1, is_selected_2, sample_rate_1, make_mesh_elements(1), scale_1));
    XCTAssertTrue(
        module_content(id_1, range_1, track_1, is_selected_1, sample_rate_1, make_mesh_elements(1), scale_1) !=
        module_content(id_1, range_1, track_1, is_selected_1, sample_rate_2, make_mesh_elements(1), scale_1));
    XCTAssertTrue(
        module_content(id_1, range_1, track_1, is_selected_1, sample_rate_1, make_mesh_elements(1), scale_1) !=
        module_content(id_1, range_1, track_1, is_selected_1, sample_rate_1, make_mesh_elements(2), scale_1));
    XCTAssertTrue(
        module_content(id_1, range_1, track_1, is_selected_1, sample_rate_1, make_mesh_elements(1), scale_1) !=
        module_content(id_1, range_1, track_1, is_selected_1, sample_rate_1, make_mesh_elements(1), scale_2));
}

@end

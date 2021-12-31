//
//  ae_module_location_tests.mm
//

#import <XCTest/XCTest.h>
#include <audio_editor_core/ae_module_location.h>

using namespace yas;
using namespace yas::ae;

@interface ae_module_location_tests : XCTestCase

@end

@implementation ae_module_location_tests

- (void)test_equal {
    identifier const id_1;
    identifier const id_2;
    proc::time::range const range_1(0, 1);
    proc::time::range const range_2(0, 2);
    proc::frame_index_t const file_frame_1 = 1;
    proc::frame_index_t const file_frame_2 = 2;
    uint32_t const sample_rate_1 = 1;
    uint32_t const sample_rate_2 = 2;
    std::vector<std::optional<module_location::mesh_element>> mesh_elements_1{{{.range = {1, 1}}}};
    std::vector<std::optional<module_location::mesh_element>> mesh_elements_2{{{.range = {1, 2}}}};
    float const width_per_sec_1 = 1.0f;
    float const width_per_sec_2 = 2.0f;

    XCTAssertTrue(
        module_location::make_value(id_1, range_1, file_frame_1, sample_rate_1, mesh_elements_1, width_per_sec_1) ==
        module_location::make_value(id_1, range_1, file_frame_1, sample_rate_1, mesh_elements_1, width_per_sec_1));
    XCTAssertFalse(
        module_location::make_value(id_1, range_1, file_frame_1, sample_rate_1, mesh_elements_1, width_per_sec_1) ==
        module_location::make_value(id_2, range_1, file_frame_1, sample_rate_1, mesh_elements_1, width_per_sec_1));
    XCTAssertFalse(
        module_location::make_value(id_1, range_1, file_frame_1, sample_rate_1, mesh_elements_1, width_per_sec_1) ==
        module_location::make_value(id_1, range_2, file_frame_1, sample_rate_1, mesh_elements_1, width_per_sec_1));
    XCTAssertFalse(
        module_location::make_value(id_1, range_1, file_frame_1, sample_rate_1, mesh_elements_1, width_per_sec_1) ==
        module_location::make_value(id_1, range_1, file_frame_2, sample_rate_1, mesh_elements_1, width_per_sec_1));
    XCTAssertFalse(
        module_location::make_value(id_1, range_1, file_frame_1, sample_rate_1, mesh_elements_1, width_per_sec_1) ==
        module_location::make_value(id_1, range_1, file_frame_1, sample_rate_2, mesh_elements_1, width_per_sec_1));
    XCTAssertFalse(
        module_location::make_value(id_1, range_1, file_frame_1, sample_rate_1, mesh_elements_1, width_per_sec_1) ==
        module_location::make_value(id_1, range_1, file_frame_1, sample_rate_1, mesh_elements_2, width_per_sec_1));
    XCTAssertFalse(
        module_location::make_value(id_1, range_1, file_frame_1, sample_rate_1, mesh_elements_1, width_per_sec_1) ==
        module_location::make_value(id_1, range_1, file_frame_1, sample_rate_1, mesh_elements_1, width_per_sec_2));

    XCTAssertFalse(
        module_location::make_value(id_1, range_1, file_frame_1, sample_rate_1, mesh_elements_1, width_per_sec_1) !=
        module_location::make_value(id_1, range_1, file_frame_1, sample_rate_1, mesh_elements_1, width_per_sec_1));
    XCTAssertTrue(
        module_location::make_value(id_1, range_1, file_frame_1, sample_rate_1, mesh_elements_1, width_per_sec_1) !=
        module_location::make_value(id_2, range_1, file_frame_1, sample_rate_1, mesh_elements_1, width_per_sec_1));
    XCTAssertTrue(
        module_location::make_value(id_1, range_1, file_frame_1, sample_rate_1, mesh_elements_1, width_per_sec_1) !=
        module_location::make_value(id_1, range_2, file_frame_1, sample_rate_1, mesh_elements_1, width_per_sec_1));
    XCTAssertTrue(
        module_location::make_value(id_1, range_1, file_frame_1, sample_rate_1, mesh_elements_1, width_per_sec_1) !=
        module_location::make_value(id_1, range_1, file_frame_2, sample_rate_1, mesh_elements_1, width_per_sec_1));
    XCTAssertTrue(
        module_location::make_value(id_1, range_1, file_frame_1, sample_rate_1, mesh_elements_1, width_per_sec_1) !=
        module_location::make_value(id_1, range_1, file_frame_1, sample_rate_2, mesh_elements_1, width_per_sec_1));
    XCTAssertTrue(
        module_location::make_value(id_1, range_1, file_frame_1, sample_rate_1, mesh_elements_1, width_per_sec_1) !=
        module_location::make_value(id_1, range_1, file_frame_1, sample_rate_1, mesh_elements_2, width_per_sec_1));
    XCTAssertTrue(
        module_location::make_value(id_1, range_1, file_frame_1, sample_rate_1, mesh_elements_1, width_per_sec_1) !=
        module_location::make_value(id_1, range_1, file_frame_1, sample_rate_1, mesh_elements_1, width_per_sec_2));
}

@end

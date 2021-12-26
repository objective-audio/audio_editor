//
//  ae_marker_location_tests.mm
//

#import <XCTest/XCTest.h>
#include <audio_editor_core/ae_marker_location.h>

using namespace yas;
using namespace yas::ae;

@interface ae_marker_location_tests : XCTestCase

@end

@implementation ae_marker_location_tests

- (void)test_equal {
    identifier const id_1;
    identifier const id_2;
    proc::frame_index_t const frame_1 = 1;
    proc::frame_index_t const frame_2 = 2;
    uint32_t const sample_rate_1 = 1;
    uint32_t const sample_rate_2 = 2;
    ui::size const scale_1{.width = 1.0f, .height = 1.0f};
    ui::size const scale_2{.width = 1.0f, .height = 2.0f};

    XCTAssertTrue(marker_location::make_value(id_1, frame_1, sample_rate_1, scale_1) ==
                  marker_location::make_value(id_1, frame_1, sample_rate_1, scale_1));
    XCTAssertFalse(marker_location::make_value(id_1, frame_1, sample_rate_1, scale_1) ==
                   marker_location::make_value(id_2, frame_1, sample_rate_1, scale_1));
    XCTAssertFalse(marker_location::make_value(id_1, frame_1, sample_rate_1, scale_1) ==
                   marker_location::make_value(id_1, frame_2, sample_rate_1, scale_1));
    XCTAssertFalse(marker_location::make_value(id_1, frame_1, sample_rate_1, scale_1) ==
                   marker_location::make_value(id_1, frame_1, sample_rate_2, scale_1));
    XCTAssertFalse(marker_location::make_value(id_1, frame_1, sample_rate_1, scale_1) ==
                   marker_location::make_value(id_1, frame_1, sample_rate_1, scale_2));

    XCTAssertFalse(marker_location::make_value(id_1, frame_1, sample_rate_1, scale_1) !=
                   marker_location::make_value(id_1, frame_1, sample_rate_1, scale_1));
    XCTAssertTrue(marker_location::make_value(id_1, frame_1, sample_rate_1, scale_1) !=
                  marker_location::make_value(id_2, frame_1, sample_rate_1, scale_1));
    XCTAssertTrue(marker_location::make_value(id_1, frame_1, sample_rate_1, scale_1) !=
                  marker_location::make_value(id_1, frame_2, sample_rate_1, scale_1));
    XCTAssertTrue(marker_location::make_value(id_1, frame_1, sample_rate_1, scale_1) !=
                  marker_location::make_value(id_1, frame_1, sample_rate_2, scale_1));
    XCTAssertTrue(marker_location::make_value(id_1, frame_1, sample_rate_1, scale_1) !=
                  marker_location::make_value(id_1, frame_1, sample_rate_1, scale_2));
}

@end

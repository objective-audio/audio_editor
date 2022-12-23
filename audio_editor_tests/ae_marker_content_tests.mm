//
//  ae_marker_content_tests.mm
//

#import <XCTest/XCTest.h>
#include <audio_editor_core/ae_marker_content.h>

using namespace yas;
using namespace yas::ae;

@interface ae_marker_content_tests : XCTestCase

@end

@implementation ae_marker_content_tests

- (void)test_equal {
    auto const id_1 = db::make_temporary_id();
    auto const id_2 = db::make_temporary_id();
    frame_index_t const frame_1 = 1;
    frame_index_t const frame_2 = 2;
    uint32_t const sample_rate_1 = 1;
    uint32_t const sample_rate_2 = 2;
    float const scale_1 = 1.0f;
    float const scale_2 = 2.0f;
    std::string const name_1 = "test_name_1";
    std::string const name_2 = "test_name_2";
    bool const is_selected_1 = false;
    bool const is_selected_2 = true;

    XCTAssertTrue(marker_content(id_1, frame_1, sample_rate_1, scale_1, name_1, is_selected_1) ==
                  marker_content(id_1, frame_1, sample_rate_1, scale_1, name_1, is_selected_1));
    XCTAssertFalse(marker_content(id_1, frame_1, sample_rate_1, scale_1, name_1, is_selected_1) ==
                   marker_content(id_2, frame_1, sample_rate_1, scale_1, name_1, is_selected_1));
    XCTAssertFalse(marker_content(id_1, frame_1, sample_rate_1, scale_1, name_1, is_selected_1) ==
                   marker_content(id_1, frame_2, sample_rate_1, scale_1, name_1, is_selected_1));
    XCTAssertFalse(marker_content(id_1, frame_1, sample_rate_1, scale_1, name_1, is_selected_1) ==
                   marker_content(id_1, frame_1, sample_rate_2, scale_1, name_1, is_selected_1));
    XCTAssertFalse(marker_content(id_1, frame_1, sample_rate_1, scale_1, name_1, is_selected_1) ==
                   marker_content(id_1, frame_1, sample_rate_1, scale_2, name_1, is_selected_1));
    XCTAssertFalse(marker_content(id_1, frame_1, sample_rate_1, scale_1, name_1, is_selected_1) ==
                   marker_content(id_1, frame_1, sample_rate_1, scale_1, name_2, is_selected_1));
    XCTAssertFalse(marker_content(id_1, frame_1, sample_rate_1, scale_1, name_1, is_selected_1) ==
                   marker_content(id_1, frame_1, sample_rate_1, scale_1, name_1, is_selected_2));

    XCTAssertFalse(marker_content(id_1, frame_1, sample_rate_1, scale_1, name_1, is_selected_1) !=
                   marker_content(id_1, frame_1, sample_rate_1, scale_1, name_1, is_selected_1));
    XCTAssertTrue(marker_content(id_1, frame_1, sample_rate_1, scale_1, name_1, is_selected_1) !=
                  marker_content(id_2, frame_1, sample_rate_1, scale_1, name_1, is_selected_1));
    XCTAssertTrue(marker_content(id_1, frame_1, sample_rate_1, scale_1, name_1, is_selected_1) !=
                  marker_content(id_1, frame_2, sample_rate_1, scale_1, name_1, is_selected_1));
    XCTAssertTrue(marker_content(id_1, frame_1, sample_rate_1, scale_1, name_1, is_selected_1) !=
                  marker_content(id_1, frame_1, sample_rate_2, scale_1, name_1, is_selected_1));
    XCTAssertTrue(marker_content(id_1, frame_1, sample_rate_1, scale_1, name_1, is_selected_1) !=
                  marker_content(id_1, frame_1, sample_rate_1, scale_2, name_1, is_selected_1));
    XCTAssertTrue(marker_content(id_1, frame_1, sample_rate_1, scale_1, name_1, is_selected_1) !=
                  marker_content(id_1, frame_1, sample_rate_1, scale_1, name_2, is_selected_1));
    XCTAssertTrue(marker_content(id_1, frame_1, sample_rate_1, scale_1, name_1, is_selected_1) !=
                  marker_content(id_1, frame_1, sample_rate_1, scale_1, name_1, is_selected_2));
}

@end

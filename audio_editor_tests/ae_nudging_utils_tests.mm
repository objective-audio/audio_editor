//
//  ae_nudging_utils_tests.mm
//

#import <XCTest/XCTest.h>
#include <audio_editor_core/ae_nudging_utils.h>

using namespace yas;
using namespace yas::ae;

@interface ae_nudging_utils_tests : XCTestCase

@end

@implementation ae_nudging_utils_tests

- (void)test_to_sample_count {
    XCTAssertEqual(nudging_utils::to_sample_count(nudging_kind::sample, 48000), 1);
    XCTAssertEqual(nudging_utils::to_sample_count(nudging_kind::sample, 96000), 1);

    XCTAssertEqual(nudging_utils::to_sample_count(nudging_kind::milisecond, 48000), 48);
    XCTAssertEqual(nudging_utils::to_sample_count(nudging_kind::milisecond, 96000), 96);

    XCTAssertEqual(nudging_utils::to_sample_count(nudging_kind::second, 48000), 48000);
    XCTAssertEqual(nudging_utils::to_sample_count(nudging_kind::second, 96000), 96000);
}

@end

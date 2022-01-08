//
//  ae_nudging_utils_tests.mm
//

#import <XCTest/XCTest.h>
#include <audio_editor_core/ae_nudging_utils.h>
#include "ae_nudging_test_utils.h"

using namespace yas;
using namespace yas::ae;

@interface ae_nudging_utils_tests : XCTestCase

@end

@implementation ae_nudging_utils_tests

- (void)test_to_sample_count {
    auto const timing48000 = std::make_shared<test_utils::timing_stub>(48000, 1);
    auto const timing96000 = std::make_shared<test_utils::timing_stub>(96000, 1);

    XCTAssertEqual(nudging_utils::to_sample_count(nudging_kind::fraction, timing48000), 1);
    XCTAssertEqual(nudging_utils::to_sample_count(nudging_kind::fraction, timing96000), 1);

    XCTAssertEqual(nudging_utils::to_sample_count(nudging_kind::second, timing48000), 48000);
    XCTAssertEqual(nudging_utils::to_sample_count(nudging_kind::second, timing96000), 96000);
}

@end

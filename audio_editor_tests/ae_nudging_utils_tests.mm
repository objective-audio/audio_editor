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

- (void)test_offset_components {
    XCTAssertEqual(nudging_utils::offset_components(false, nudging_kind::minute),
                   (timing_components{.is_minus = false, .minutes = 1}));
    XCTAssertEqual(nudging_utils::offset_components(false, nudging_kind::second),
                   (timing_components{.is_minus = false, .seconds = 1}));
    XCTAssertEqual(nudging_utils::offset_components(false, nudging_kind::fraction),
                   (timing_components{.is_minus = false, .fraction = 1}));

    XCTAssertEqual(nudging_utils::offset_components(true, nudging_kind::minute),
                   (timing_components{.is_minus = true, .minutes = 1}));
    XCTAssertEqual(nudging_utils::offset_components(true, nudging_kind::second),
                   (timing_components{.is_minus = true, .seconds = 1}));
    XCTAssertEqual(nudging_utils::offset_components(true, nudging_kind::fraction),
                   (timing_components{.is_minus = true, .fraction = 1}));
}

@end

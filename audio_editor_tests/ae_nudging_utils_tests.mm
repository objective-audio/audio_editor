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
    XCTAssertEqual(nudging_utils::offset_components(false, 1, 2, 10000),
                   (timing_components{{.is_minus = false, .minutes = 1, .fraction_unit_size = 10000}}));
    XCTAssertEqual(nudging_utils::offset_components(false, 10, 1, 10000),
                   (timing_components{{.is_minus = false, .seconds = 10, .fraction_unit_size = 10000}}));
    XCTAssertEqual(nudging_utils::offset_components(false, 100, 0, 10000),
                   (timing_components{{.is_minus = false, .fraction = 100, .fraction_unit_size = 10000}}));

    XCTAssertEqual(nudging_utils::offset_components(true, 1, 2, 10000),
                   (timing_components{{.is_minus = true, .minutes = 1, .fraction_unit_size = 10000}}));
    XCTAssertEqual(nudging_utils::offset_components(true, 10, 1, 10000),
                   (timing_components{{.is_minus = true, .seconds = 10, .fraction_unit_size = 10000}}));
    XCTAssertEqual(nudging_utils::offset_components(true, 100, 0, 10000),
                   (timing_components{{.is_minus = true, .fraction = 100, .fraction_unit_size = 10000}}));
}

@end

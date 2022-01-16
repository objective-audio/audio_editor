//
//  ae_timing_components_tests.mm
//

#import <XCTest/XCTest.h>
#include <audio_editor_core/ae_timing_types.h>

using namespace yas;
using namespace yas::ae;

@interface ae_timing_components_tests : XCTestCase

@end

@implementation ae_timing_components_tests

- (void)test_timing_components_is_zero {
    XCTAssertTrue(
        (timing_components{{.is_minus = false, .hours = 0, .minutes = 0, .seconds = 0, .fraction = 0}}).is_zero());
    XCTAssertTrue(
        (timing_components{{.is_minus = false, .hours = 0, .minutes = 0, .seconds = 0, .fraction = 0}}).is_zero());
    XCTAssertTrue(
        (timing_components{{.is_minus = true, .hours = 0, .minutes = 0, .seconds = 0, .fraction = 0}}).is_zero());

    XCTAssertFalse(
        (timing_components{{.is_minus = false, .hours = 0, .minutes = 0, .seconds = 0, .fraction = 1}}).is_zero());
    XCTAssertFalse(
        (timing_components{{.is_minus = false, .hours = 0, .minutes = 0, .seconds = 1, .fraction = 0}}).is_zero());
    XCTAssertFalse(
        (timing_components{{.is_minus = false, .hours = 0, .minutes = 1, .seconds = 0, .fraction = 0}}).is_zero());
    XCTAssertFalse(
        (timing_components{{.is_minus = false, .hours = 1, .minutes = 0, .seconds = 0, .fraction = 0}}).is_zero());
}

- (void)test_abs {
    XCTAssertTrue(
        (timing_components{{.is_minus = true, .hours = 1, .minutes = 23, .seconds = 45, .fraction = 6789}}).abs() ==
        (timing_components{{.is_minus = false, .hours = 1, .minutes = 23, .seconds = 45, .fraction = 6789}}));
    XCTAssertTrue(
        (timing_components{{.is_minus = false, .hours = 1, .minutes = 23, .seconds = 45, .fraction = 6789}}).abs() ==
        (timing_components{{.is_minus = false, .hours = 1, .minutes = 23, .seconds = 45, .fraction = 6789}}));
}

- (void)test_timing_components_equal {
    XCTAssertTrue((timing_components{{.is_minus = false, .hours = 0, .minutes = 0, .seconds = 0, .fraction = 0}}) ==
                  (timing_components{{.is_minus = false, .hours = 0, .minutes = 0, .seconds = 0, .fraction = 0}}));
    XCTAssertTrue((timing_components{{.is_minus = false, .hours = 0, .minutes = 0, .seconds = 0, .fraction = 0}}) ==
                  (timing_components{{.is_minus = true, .hours = 0, .minutes = 0, .seconds = 0, .fraction = 0}}));

    XCTAssertTrue((timing_components{{.is_minus = true, .hours = 1, .minutes = 23, .seconds = 45, .fraction = 6789}}) ==
                  (timing_components{{.is_minus = true, .hours = 1, .minutes = 23, .seconds = 45, .fraction = 6789}}));

    XCTAssertFalse(
        (timing_components{{.is_minus = true, .hours = 1, .minutes = 23, .seconds = 45, .fraction = 6789}}) ==
        (timing_components{{.is_minus = false, .hours = 1, .minutes = 23, .seconds = 45, .fraction = 6789}}));
    XCTAssertFalse(
        (timing_components{{.is_minus = true, .hours = 1, .minutes = 23, .seconds = 45, .fraction = 6789}}) ==
        (timing_components{{.is_minus = true, .hours = 0, .minutes = 23, .seconds = 45, .fraction = 6789}}));
    XCTAssertFalse(
        (timing_components{{.is_minus = true, .hours = 1, .minutes = 23, .seconds = 45, .fraction = 6789}}) ==
        (timing_components{{.is_minus = true, .hours = 1, .minutes = 0, .seconds = 45, .fraction = 6789}}));
    XCTAssertFalse(
        (timing_components{{.is_minus = true, .hours = 1, .minutes = 23, .seconds = 45, .fraction = 6789}}) ==
        (timing_components{{.is_minus = true, .hours = 1, .minutes = 23, .seconds = 0, .fraction = 6789}}));
    XCTAssertFalse(
        (timing_components{{.is_minus = true, .hours = 1, .minutes = 23, .seconds = 45, .fraction = 6789}}) ==
        (timing_components{{.is_minus = true, .hours = 1, .minutes = 23, .seconds = 45, .fraction = 0}}));
}

- (void)test_components_less_than {
    XCTAssertTrue((timing_components{{.is_minus = false, .hours = 0, .minutes = 0, .seconds = 0, .fraction = 0}}) <
                  (timing_components{{.is_minus = false, .hours = 0, .minutes = 0, .seconds = 0, .fraction = 1}}));
    XCTAssertTrue((timing_components{{.is_minus = false, .hours = 0, .minutes = 0, .seconds = 0, .fraction = 99999}}) <
                  (timing_components{{.is_minus = false, .hours = 0, .minutes = 0, .seconds = 1, .fraction = 0}}));
    XCTAssertTrue((timing_components{{.is_minus = false, .hours = 0, .minutes = 0, .seconds = 59, .fraction = 0}}) <
                  (timing_components{{.is_minus = false, .hours = 0, .minutes = 1, .seconds = 0, .fraction = 0}}));
    XCTAssertTrue((timing_components{{.is_minus = false, .hours = 0, .minutes = 59, .seconds = 0, .fraction = 0}}) <
                  (timing_components{{.is_minus = false, .hours = 1, .minutes = 0, .seconds = 0, .fraction = 0}}));

    XCTAssertFalse((timing_components{{.is_minus = true, .hours = 0, .minutes = 0, .seconds = 0, .fraction = 0}}) <
                   (timing_components{{.is_minus = true, .hours = 0, .minutes = 0, .seconds = 0, .fraction = 1}}));
    XCTAssertFalse((timing_components{{.is_minus = true, .hours = 0, .minutes = 0, .seconds = 0, .fraction = 99999}}) <
                   (timing_components{{.is_minus = true, .hours = 0, .minutes = 0, .seconds = 1, .fraction = 0}}));
    XCTAssertFalse((timing_components{{.is_minus = true, .hours = 0, .minutes = 0, .seconds = 59, .fraction = 0}}) <
                   (timing_components{{.is_minus = true, .hours = 0, .minutes = 1, .seconds = 0, .fraction = 0}}));
    XCTAssertFalse((timing_components{{.is_minus = true, .hours = 0, .minutes = 59, .seconds = 0, .fraction = 0}}) <
                   (timing_components{{.is_minus = true, .hours = 1, .minutes = 0, .seconds = 0, .fraction = 0}}));

    XCTAssertTrue((timing_components{{.is_minus = true, .hours = 0, .minutes = 0, .seconds = 0, .fraction = 2}}) <
                  (timing_components{{.is_minus = false, .hours = 0, .minutes = 0, .seconds = 0, .fraction = 1}}));
    XCTAssertFalse((timing_components{{.is_minus = false, .hours = 0, .minutes = 0, .seconds = 0, .fraction = 1}}) <
                   (timing_components{{.is_minus = true, .hours = 0, .minutes = 0, .seconds = 0, .fraction = 2}}));
}

@end

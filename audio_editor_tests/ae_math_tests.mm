//
//  ae_math_tests.mm
//

#import <XCTest/XCTest.h>
#include <audio_editor_core/ae_math.h>

using namespace yas;
using namespace yas::ae;

@interface ae_math_tests : XCTestCase

@end

@implementation ae_math_tests

- (void)test_fraction_digits {
    XCTAssertEqual(math::fraction_digits(1), 0);
    XCTAssertEqual(math::fraction_digits(2), 1);
    XCTAssertEqual(math::fraction_digits(9), 1);
    XCTAssertEqual(math::fraction_digits(10), 1);
    XCTAssertEqual(math::fraction_digits(11), 2);
    XCTAssertEqual(math::fraction_digits(19), 2);
    XCTAssertEqual(math::fraction_digits(20), 2);
    XCTAssertEqual(math::fraction_digits(21), 2);
    XCTAssertEqual(math::fraction_digits(99), 2);
    XCTAssertEqual(math::fraction_digits(100), 2);
    XCTAssertEqual(math::fraction_digits(101), 3);
    XCTAssertEqual(math::fraction_digits(999), 3);
    XCTAssertEqual(math::fraction_digits(1000), 3);
    XCTAssertEqual(math::fraction_digits(1001), 4);
}

@end

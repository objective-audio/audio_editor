//
//  ae_math_tests.mm
//

#import <XCTest/XCTest.h>
#include <ae-core/global/utils/ae_math.h>

using namespace yas;
using namespace yas::ae;

@interface ae_math_tests : XCTestCase

@end

@implementation ae_math_tests

- (void)test_decimal_digits_from_size {
    XCTAssertEqual(math::decimal_digits_from_size(1), 0);
    XCTAssertEqual(math::decimal_digits_from_size(2), 1);
    XCTAssertEqual(math::decimal_digits_from_size(9), 1);
    XCTAssertEqual(math::decimal_digits_from_size(10), 1);
    XCTAssertEqual(math::decimal_digits_from_size(11), 2);
    XCTAssertEqual(math::decimal_digits_from_size(19), 2);
    XCTAssertEqual(math::decimal_digits_from_size(20), 2);
    XCTAssertEqual(math::decimal_digits_from_size(21), 2);
    XCTAssertEqual(math::decimal_digits_from_size(99), 2);
    XCTAssertEqual(math::decimal_digits_from_size(100), 2);
    XCTAssertEqual(math::decimal_digits_from_size(101), 3);
    XCTAssertEqual(math::decimal_digits_from_size(999), 3);
    XCTAssertEqual(math::decimal_digits_from_size(1000), 3);
    XCTAssertEqual(math::decimal_digits_from_size(1001), 4);
}

- (void)test_decimal_max_size_of_digits {
    XCTAssertEqual(math::decimal_max_size_of_digits(1), 10);
    XCTAssertEqual(math::decimal_max_size_of_digits(2), 100);
    XCTAssertEqual(math::decimal_max_size_of_digits(3), 1000);
}

@end

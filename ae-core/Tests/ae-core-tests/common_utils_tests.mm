//
//  common_utils_tests.mm
//

#import <XCTest/XCTest.h>

#import <ae-core/global/utils/common_utils.h>

using namespace yas;
using namespace yas::ae;

@interface common_utils_tests : XCTestCase

@end

@implementation common_utils_tests

- (void)test_reserving_count {
    XCTAssertEqual(common_utils::reserving_count(0, 2), 2);
    XCTAssertEqual(common_utils::reserving_count(1, 2), 2);
    XCTAssertEqual(common_utils::reserving_count(2, 2), 2);
    XCTAssertEqual(common_utils::reserving_count(3, 2), 4);
    XCTAssertEqual(common_utils::reserving_count(4, 2), 4);
    XCTAssertEqual(common_utils::reserving_count(5, 2), 6);
}

@end

//
//  ae_module_utils_tests.mm
//

#import <XCTest/XCTest.h>
#import <ae-core/project_editing/value_types/ae_module_utils.h>

using namespace yas;
using namespace yas::ae;

@interface ae_module_utils_tests : XCTestCase

@end

@implementation ae_module_utils_tests

- (void)test_can_split_time_range {
    time::range const range{1, 3};

    XCTAssertFalse(module_utils::can_split_time_range(range, 0));
    XCTAssertFalse(module_utils::can_split_time_range(range, 1));
    XCTAssertTrue(module_utils::can_split_time_range(range, 2));
    XCTAssertTrue(module_utils::can_split_time_range(range, 3));
    XCTAssertFalse(module_utils::can_split_time_range(range, 4));
}

@end

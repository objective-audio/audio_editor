//
//  ae_file_module_utils_tests.mm
//

#import <XCTest/XCTest.h>
#import <audio_editor_core/ae_file_module_utils.h>

using namespace yas;
using namespace yas::ae;

@interface ae_file_module_utils_tests : XCTestCase

@end

@implementation ae_file_module_utils_tests

- (void)test_can_split_time_range {
    proc::time::range const range{1, 3};

    XCTAssertFalse(file_module_utils::can_split_time_range(range, 0));
    XCTAssertFalse(file_module_utils::can_split_time_range(range, 1));
    XCTAssertTrue(file_module_utils::can_split_time_range(range, 2));
    XCTAssertTrue(file_module_utils::can_split_time_range(range, 3));
    XCTAssertFalse(file_module_utils::can_split_time_range(range, 4));
}

@end

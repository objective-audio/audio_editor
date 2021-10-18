//
//  ae_common_utils_tests.mm
//

#import <XCTest/XCTest.h>

#import <audio_editor_core/ae_common_utils.h>

using namespace yas;
using namespace yas::ae;

@interface ae_common_utils_tests : XCTestCase

@end

@implementation ae_common_utils_tests

- (void)test_reserving_count {
    XCTAssertEqual(common_utils::reserving_count(0, 2), 0);
    XCTAssertEqual(common_utils::reserving_count(1, 2), 2);
    XCTAssertEqual(common_utils::reserving_count(2, 2), 2);
    XCTAssertEqual(common_utils::reserving_count(3, 2), 4);
    XCTAssertEqual(common_utils::reserving_count(4, 2), 4);
    XCTAssertEqual(common_utils::reserving_count(5, 2), 6);
}

@end

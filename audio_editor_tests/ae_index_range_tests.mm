//
//  ae_index_range_tests.mm
//

#import <XCTest/XCTest.h>
#include <audio_editor_core/ae_index_range.h>

using namespace yas;
using namespace yas::ae;

@interface ae_index_range_tests : XCTestCase

@end

@implementation ae_index_range_tests

- (void)test_equal {
    index_range const index_range_1a{.index = 0, .length = 1};
    index_range const index_range_1b{.index = 0, .length = 1};
    index_range const index_range_2{.index = 1, .length = 1};
    index_range const index_range_3{.index = 0, .length = 2};

    XCTAssertTrue(index_range_1a == index_range_1a);
    XCTAssertTrue(index_range_1a == index_range_1b);
    XCTAssertFalse(index_range_1a == index_range_2);
    XCTAssertFalse(index_range_1a == index_range_3);

    XCTAssertFalse(index_range_1a != index_range_1a);
    XCTAssertFalse(index_range_1a != index_range_1b);
    XCTAssertTrue(index_range_1a != index_range_2);
    XCTAssertTrue(index_range_1a != index_range_3);
}

@end

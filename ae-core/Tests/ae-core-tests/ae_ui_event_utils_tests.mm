//
//  ae_ui_event_utils_tests.mm
//

#import <XCTest/XCTest.h>
#include <ae-core/ui_base/utils/ae_ui_event_utils.h>

using namespace yas;
using namespace yas::ae;

@interface ae_ui_event_utils_tests : XCTestCase

@end

@implementation ae_ui_event_utils_tests

- (void)setUp {
}

- (void)tearDown {
}

- (void)test_to_event_string_from_point {
    XCTAssertEqual(ui_event_utils::to_event_string(ui::point{.x = 1.0f, .y = -2.0f}), "1.000000,-2.000000");
}

- (void)test_to_point_from_event_string {
    XCTAssertTrue(ui_event_utils::to_position_from_event_string("1,-2") == (ui::point{.x = 1.0f, .y = -2.0f}));
}

@end

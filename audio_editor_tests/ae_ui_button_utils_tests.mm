//
//  ae_ui_button_utils_tests.mm
//

#import <XCTest/XCTest.h>
#include <audio_editor_core/ae_ui_button_utils.h>

#include <iostream>

using namespace yas;
using namespace yas::ae;

@interface ae_ui_button_utils_tests : XCTestCase

@end

@implementation ae_ui_button_utils_tests

- (void)setUp {
}

- (void)tearDown {
}

- (void)test_to_event_string_from_point {
    XCTAssertEqual(ui_button_utils::to_event_string(ui::point{.x = 1.0f, .y = -2.0f}), "1.000000,-2.000000");
}

- (void)test_to_point_from_event_string {
    XCTAssertTrue(ui_button_utils::to_position_from_event_string("1,-2") == (ui::point{.x = 1.0f, .y = -2.0f}));
}

@end

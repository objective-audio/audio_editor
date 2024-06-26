//
//  color_tests.mm
//

#import <XCTest/XCTest.h>
#include <ae-core/app/features/color.h>

using namespace yas;
using namespace yas::ae;

@interface color_tests : XCTestCase

@end

@implementation color_tests

- (void)test_color {
    auto const color = std::make_shared<ae::color>();

    XCTAssertNoThrow(color->background());

    XCTAssertNoThrow(color->playing_line_pausing());
    XCTAssertNoThrow(color->playing_line_playing());
    XCTAssertNoThrow(color->playing_line_scrolling());

    XCTAssertNoThrow(color->module_frame());
    XCTAssertNoThrow(color->module_bg());
    XCTAssertNoThrow(color->selected_module_bg());
    XCTAssertNoThrow(color->module_name());
    XCTAssertNoThrow(color->waveform());

    XCTAssertNoThrow(color->range_selection_fill());
    XCTAssertNoThrow(color->range_selection_frame());

    XCTAssertNoThrow(color->pasting_module_frame());

    XCTAssertNoThrow(color->edge_line());
    XCTAssertNoThrow(color->edge_text());

    XCTAssertNoThrow(color->marker_square());
    XCTAssertNoThrow(color->selected_marker_square());
    XCTAssertNoThrow(color->marker_line());
    XCTAssertNoThrow(color->marker_text());
    XCTAssertNoThrow(color->selected_marker_text());

    XCTAssertNoThrow(color->pasting_marker_line());

    XCTAssertNoThrow(color->track_square());
    XCTAssertNoThrow(color->selected_track_square());
    XCTAssertNoThrow(color->track_bg());
    XCTAssertNoThrow(color->selected_track_bg());

    XCTAssertNoThrow(color->modal_bg());

    XCTAssertNoThrow(color->time_text());
    XCTAssertNoThrow(color->time_selected_text());
    XCTAssertNoThrow(color->time_nudging_line());
    XCTAssertNoThrow(color->time_bg());
    XCTAssertNoThrow(color->time_bg_tracking());
    XCTAssertNoThrow(color->time_unit());
    XCTAssertNoThrow(color->time_unit_tracking());
    XCTAssertNoThrow(color->time_unit_selected());
    XCTAssertNoThrow(color->time_unit_selected_tracking());

    XCTAssertNoThrow(color->debug_text());
}

@end

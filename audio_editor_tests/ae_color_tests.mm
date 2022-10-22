//
//  ae_color_tests.mm
//

#import <XCTest/XCTest.h>
#include <audio_editor_core/ae_color.h>

using namespace yas;
using namespace yas::ae;

@interface ae_color_tests : XCTestCase

@end

@implementation ae_color_tests

- (void)test_color {
    auto const color = ae::color::make_shared();

    XCTAssertNoThrow(color->background());

    XCTAssertNoThrow(color->playing_line_pausing());
    XCTAssertNoThrow(color->playing_line_playing());
    XCTAssertNoThrow(color->playing_line_scroling());

    XCTAssertNoThrow(color->module_frame());
    XCTAssertNoThrow(color->module_bg());
    XCTAssertNoThrow(color->module_name());
    XCTAssertNoThrow(color->waveform());

    XCTAssertNoThrow(color->selected_module_frame());

    XCTAssertNoThrow(color->pasting_module_frame());

    XCTAssertNoThrow(color->edge_line());
    XCTAssertNoThrow(color->edge_text());

    XCTAssertNoThrow(color->marker_square());
    XCTAssertNoThrow(color->marker_line());
    XCTAssertNoThrow(color->marker_text());

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

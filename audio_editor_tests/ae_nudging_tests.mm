//
//  ae_nudging_tests.mm
//

#import <XCTest/XCTest.h>
#include <audio_editor_core/ae_nudging.h>
#include <audio_editor_core/ae_timing_utils.h>
#include "ae_nudging_test_utils.h"

using namespace yas;
using namespace yas::ae;

@interface ae_nudging_tests : XCTestCase

@end

@implementation ae_nudging_tests

- (void)test_initial {
    auto const timing = std::make_shared<test_utils::timing_stub>(48000);
    auto const nudging = nudging::make_shared(timing);

    XCTAssertEqual(nudging->unit_index(), 0);
}

- (void)test_rotate_next {
    auto const timing = std::make_shared<test_utils::timing_stub>(48000);
    auto const nudging = nudging::make_shared(timing);

    XCTAssertEqual(nudging->unit_index(), 0);

    nudging->rotate_next_unit();

    XCTAssertEqual(nudging->unit_index(), 3);

    nudging->rotate_next_unit();

    XCTAssertEqual(nudging->unit_index(), 2);

    nudging->rotate_next_unit();

    XCTAssertEqual(nudging->unit_index(), 1);
}

- (void)test_rotate_previous {
    auto const timing = std::make_shared<test_utils::timing_stub>(48000);
    auto const nudging = nudging::make_shared(timing);

    XCTAssertEqual(nudging->unit_index(), 0);

    nudging->rotate_previous_unit();

    XCTAssertEqual(nudging->unit_index(), 1);

    nudging->rotate_previous_unit();

    XCTAssertEqual(nudging->unit_index(), 2);

    nudging->rotate_previous_unit();

    XCTAssertEqual(nudging->unit_index(), 3);

    nudging->rotate_previous_unit();

    XCTAssertEqual(nudging->unit_index(), 0);
}

- (void)test_next_previous_frame {
    ae::timing_fraction_kind const fraction_kind = ae::timing_fraction_kind::frame30;
    sample_rate_t const sample_rate = 300;
    auto const timing = std::make_shared<test_utils::timing_stub>(sample_rate);

    timing->components_handler = [&fraction_kind, &sample_rate](frame_index_t const &frame) {
        return timing_utils::to_components(frame, fraction_kind, sample_rate);
    };

    timing->frame_handler = [&fraction_kind, &sample_rate](timing_components const &components) {
        return timing_utils::to_frame(components, fraction_kind, sample_rate);
    };

    auto const nudging = nudging::make_shared(timing);

    XCTAssertEqual(nudging->next_frame(0, 1), 10);
    XCTAssertEqual(nudging->next_frame(0, 10), 100);
    XCTAssertEqual(nudging->next_frame(0, 30), 300);

    XCTAssertEqual(nudging->previous_frame(0, 1), -10);
    XCTAssertEqual(nudging->previous_frame(0, 10), -100);
    XCTAssertEqual(nudging->previous_frame(0, 30), -300);
}

@end

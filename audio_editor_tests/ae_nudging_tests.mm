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
    auto const timing = std::make_shared<test_utils::timing_stub>(48000, timing_fraction_kind::frame30);
    auto const app_settings = std::make_shared<test_utils::app_settings_stub>();
    auto const nudging = std::make_shared<ae::nudging>(timing.get(), app_settings.get());

    XCTAssertEqual(nudging->kind(), timing_unit_kind::fraction);
    XCTAssertEqual(nudging->unit_index(), 0);
}

- (void)test_restore_kind {
    auto const timing = std::make_shared<test_utils::timing_stub>(48000, timing_fraction_kind::frame30);
    auto const app_settings = std::make_shared<test_utils::app_settings_stub>();

    app_settings->kind = timing_unit_kind::minutes;

    auto const nudging = std::make_shared<ae::nudging>(timing.get(), app_settings.get());

    XCTAssertEqual(nudging->kind(), timing_unit_kind::minutes);
}

- (void)test_rotate_next {
    auto const timing = std::make_shared<test_utils::timing_stub>(48000, timing_fraction_kind::frame30);
    auto const app_settings = std::make_shared<test_utils::app_settings_stub>();
    auto const nudging = std::make_shared<ae::nudging>(timing.get(), app_settings.get());

    XCTAssertEqual(nudging->unit_index(), 0);

    nudging->rotate_next_unit();

    XCTAssertEqual(nudging->unit_index(), 3);

    nudging->rotate_next_unit();

    XCTAssertEqual(nudging->unit_index(), 2);

    nudging->rotate_next_unit();

    XCTAssertEqual(nudging->unit_index(), 1);
}

- (void)test_rotate_previous {
    auto const timing = std::make_shared<test_utils::timing_stub>(48000, timing_fraction_kind::frame30);
    auto const app_settings = std::make_shared<test_utils::app_settings_stub>();
    auto const nudging = std::make_shared<ae::nudging>(timing.get(), app_settings.get());

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

- (void)test_next_previous_nudging_frame {
    auto const timing = std::make_shared<test_utils::timing_stub>(300, timing_fraction_kind::frame30);
    auto const app_settings = std::make_shared<test_utils::app_settings_stub>();
    auto const nudging = std::make_shared<ae::nudging>(timing.get(), app_settings.get());

    XCTAssertEqual(nudging->next_nudging_frame(0, 1), 10);
    XCTAssertEqual(nudging->next_nudging_frame(0, 10), 100);
    XCTAssertEqual(nudging->next_nudging_frame(0, 30), 300);

    XCTAssertEqual(nudging->previous_nudging_frame(0, 1), -10);
    XCTAssertEqual(nudging->previous_nudging_frame(0, 10), -100);
    XCTAssertEqual(nudging->previous_nudging_frame(0, 30), -300);
}

@end

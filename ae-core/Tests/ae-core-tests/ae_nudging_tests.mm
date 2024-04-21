//
//  ae_nudging_tests.mm
//

#import <XCTest/XCTest.h>
#include <ae-core/project_editing/features/ae_nudging.h>
#include <ae-core/project_editing/features/ae_timing_utils.h>
#include "utils/ae_nudging_test_utils.h"

using namespace yas;
using namespace yas::ae;

@interface ae_nudging_tests : XCTestCase

@end

@implementation ae_nudging_tests

- (void)test_initial {
    auto const timing = std::make_shared<test_utils::timing_stub>(48000, timing_fraction_kind::frame30);
    auto const app_settings = std::make_shared<test_utils::app_settings_stub>();

    app_settings->kind_holder->set_value(timing_unit_kind::minutes);

    auto const nudging = std::make_shared<ae::nudging>(timing.get(), app_settings.get());

    XCTAssertEqual(app_settings->timing_unit_kind(), timing_unit_kind::minutes);
}

- (void)test_rotate_next {
    auto const timing = std::make_shared<test_utils::timing_stub>(48000, timing_fraction_kind::frame30);
    auto const app_settings = std::make_shared<test_utils::app_settings_stub>();
    auto const nudging = std::make_shared<ae::nudging>(timing.get(), app_settings.get());

    XCTAssertEqual(app_settings->timing_unit_kind(), ae::timing_unit_kind::fraction);

    nudging->rotate_next_unit();

    XCTAssertEqual(app_settings->timing_unit_kind(), ae::timing_unit_kind::hours);

    nudging->rotate_next_unit();

    XCTAssertEqual(app_settings->timing_unit_kind(), ae::timing_unit_kind::minutes);

    nudging->rotate_next_unit();

    XCTAssertEqual(app_settings->timing_unit_kind(), ae::timing_unit_kind::seconds);
}

- (void)test_rotate_previous {
    auto const timing = std::make_shared<test_utils::timing_stub>(48000, timing_fraction_kind::frame30);
    auto const app_settings = std::make_shared<test_utils::app_settings_stub>();
    auto const nudging = std::make_shared<ae::nudging>(timing.get(), app_settings.get());

    XCTAssertEqual(app_settings->timing_unit_kind(), ae::timing_unit_kind::fraction);

    nudging->rotate_previous_unit();

    XCTAssertEqual(app_settings->timing_unit_kind(), ae::timing_unit_kind::seconds);

    nudging->rotate_previous_unit();

    XCTAssertEqual(app_settings->timing_unit_kind(), ae::timing_unit_kind::minutes);

    nudging->rotate_previous_unit();

    XCTAssertEqual(app_settings->timing_unit_kind(), ae::timing_unit_kind::hours);

    nudging->rotate_previous_unit();

    XCTAssertEqual(app_settings->timing_unit_kind(), ae::timing_unit_kind::fraction);
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

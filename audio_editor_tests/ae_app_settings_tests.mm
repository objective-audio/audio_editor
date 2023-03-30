//
//  ae_app_settings_tests.mm
//

#import <XCTest/XCTest.h>
#include <audio_editor_core/ae_app_settings.hpp>
#include "ae_test_utils.h"

using namespace yas;
using namespace yas::ae;

@interface ae_app_settings_tests : XCTestCase

@end

@implementation ae_app_settings_tests

- (void)setUp {
    [super setUp];

    test_utils::clear_user_defaults();
}

- (void)tearDown {
    test_utils::clear_user_defaults();

    [super tearDown];
}

- (void)test_timing_fraction_kind {
    auto const settings = std::make_shared<app_settings>();

    XCTAssertEqual(settings->timing_fraction_kind(), timing_fraction_kind::sample);

    settings->set_timing_fraction_kind(timing_fraction_kind::milisecond);

    XCTAssertEqual(settings->timing_fraction_kind(), timing_fraction_kind::milisecond);

    settings->set_timing_fraction_kind(timing_fraction_kind::frame30);

    XCTAssertEqual(settings->timing_fraction_kind(), timing_fraction_kind::frame30);

    settings->set_timing_fraction_kind(timing_fraction_kind::sample);

    XCTAssertEqual(settings->timing_fraction_kind(), timing_fraction_kind::sample);
}

- (void)test_timing_unit_kind {
    auto const settings = std::make_shared<app_settings>();

    XCTAssertEqual(settings->timing_unit_kind(), timing_unit_kind::fraction);

    settings->set_timing_unit_kind(timing_unit_kind::seconds);

    XCTAssertEqual(settings->timing_unit_kind(), timing_unit_kind::seconds);

    settings->set_timing_unit_kind(timing_unit_kind::minutes);

    XCTAssertEqual(settings->timing_unit_kind(), timing_unit_kind::minutes);

    settings->set_timing_unit_kind(timing_unit_kind::hours);

    XCTAssertEqual(settings->timing_unit_kind(), timing_unit_kind::hours);

    settings->set_timing_unit_kind(timing_unit_kind::fraction);

    XCTAssertEqual(settings->timing_unit_kind(), timing_unit_kind::fraction);
}

@end

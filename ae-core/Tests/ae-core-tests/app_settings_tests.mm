//
//  app_settings_tests.mm
//

#import <XCTest/XCTest.h>
#include <ae-core/app/features/app_settings.hpp>
#include "utils/test_utils.h"

using namespace yas;
using namespace yas::ae;

@interface app_settings_tests : XCTestCase

@end

@implementation app_settings_tests

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

- (void)test_observe_timing_fraction_kind {
    auto const settings = std::make_shared<app_settings>();

    std::vector<ae::timing_fraction_kind> kinds;

    auto canceller =
        settings->observe_timing_fraction_kind([&kinds](timing_fraction_kind const &kind) { kinds.emplace_back(kind); })
            .sync();

    XCTAssertEqual(kinds.size(), 1);
    XCTAssertEqual(kinds.at(0), timing_fraction_kind::sample);

    settings->set_timing_fraction_kind(timing_fraction_kind::milisecond);

    XCTAssertEqual(kinds.size(), 2);
    XCTAssertEqual(kinds.at(1), timing_fraction_kind::milisecond);

    canceller->cancel();
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

- (void)test_observe_timing_unit_kind {
    auto const settings = std::make_shared<app_settings>();

    std::vector<ae::timing_unit_kind> kinds;

    auto canceller =
        settings->observe_timing_unit_kind([&kinds](timing_unit_kind const &kind) { kinds.emplace_back(kind); }).sync();

    XCTAssertEqual(kinds.size(), 1);
    XCTAssertEqual(kinds.at(0), timing_unit_kind::fraction);

    settings->set_timing_unit_kind(timing_unit_kind::minutes);

    XCTAssertEqual(kinds.size(), 2);
    XCTAssertEqual(kinds.at(1), timing_unit_kind::minutes);

    canceller->cancel();
}

@end

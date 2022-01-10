//
//  ae_timing_tests.mm
//

#import <XCTest/XCTest.h>
#include <audio_editor_core/ae_timing.h>

using namespace yas;
using namespace yas::ae;

@interface ae_timing_tests : XCTestCase

@end

@implementation ae_timing_tests

- (void)test_initial {
    auto const timing = timing::make_shared(48000);

    XCTAssertEqual(timing->sample_rate(), 48000);
    XCTAssertEqual(timing->fraction_kind(), timing_fraction_kind::sample);
}

- (void)test_set_fraction {
    auto const timing = timing::make_shared(48000);

    std::vector<timing_fraction_kind> called;

    auto canceller =
        timing->observe_fraction([&called](timing_fraction_kind const &fraction) { called.emplace_back(fraction); })
            .sync();

    XCTAssertEqual(called.size(), 1);
    XCTAssertEqual(called.at(0), timing_fraction_kind::sample);
    XCTAssertEqual(timing->fraction_kind(), timing_fraction_kind::sample);
    XCTAssertEqual(timing->unit_sample_count(), 1);

    timing->set_fraction_kind(timing_fraction_kind::milisecond);

    XCTAssertEqual(called.size(), 2);
    XCTAssertEqual(called.at(1), timing_fraction_kind::milisecond);
    XCTAssertEqual(timing->fraction_kind(), timing_fraction_kind::milisecond);
    XCTAssertEqual(timing->unit_sample_count(), 48);

    timing->set_fraction_kind(timing_fraction_kind::frame30);

    XCTAssertEqual(called.size(), 3);
    XCTAssertEqual(called.at(2), timing_fraction_kind::frame30);
    XCTAssertEqual(timing->fraction_kind(), timing_fraction_kind::frame30);
    XCTAssertEqual(timing->unit_sample_count(), 1600);

    canceller->cancel();
}

- (void)test_fraction_value_of_sample {
    auto const timing = timing::make_shared(48000);

    timing->set_fraction_kind(timing_fraction_kind::sample);

    XCTAssertEqual(timing->fraction_value(0), 0);
    XCTAssertEqual(timing->fraction_value(1), 1);
    XCTAssertEqual(timing->fraction_value(47999), 47999);
    XCTAssertEqual(timing->fraction_value(48000), 0);
    XCTAssertEqual(timing->fraction_value(-1), 1);
    XCTAssertEqual(timing->fraction_value(-47999), 47999);
    XCTAssertEqual(timing->fraction_value(-48000), 0);
}

- (void)test_fraction_value_of_milisecond {
    auto const timing = timing::make_shared(48000);

    timing->set_fraction_kind(timing_fraction_kind::milisecond);

    XCTAssertEqual(timing->fraction_value(0), 0);
    XCTAssertEqual(timing->fraction_value(1), 0);
    XCTAssertEqual(timing->fraction_value(47), 0);
    XCTAssertEqual(timing->fraction_value(48), 1);
    XCTAssertEqual(timing->fraction_value(47999), 999);
    XCTAssertEqual(timing->fraction_value(48000), 0);
    XCTAssertEqual(timing->fraction_value(-1), 0);
    XCTAssertEqual(timing->fraction_value(-47), 0);
    XCTAssertEqual(timing->fraction_value(-48), 1);
    XCTAssertEqual(timing->fraction_value(-47999), 999);
    XCTAssertEqual(timing->fraction_value(-48000), 0);
}

- (void)test_fraction_value_of_frame30 {
    auto const timing = timing::make_shared(48000);

    timing->set_fraction_kind(timing_fraction_kind::frame30);

    XCTAssertEqual(timing->fraction_value(0), 0);
    XCTAssertEqual(timing->fraction_value(1), 0);
    XCTAssertEqual(timing->fraction_value(1599), 0);
    XCTAssertEqual(timing->fraction_value(1600), 1);
    XCTAssertEqual(timing->fraction_value(47999), 29);
    XCTAssertEqual(timing->fraction_value(48000), 0);
    XCTAssertEqual(timing->fraction_value(-1), 0);
    XCTAssertEqual(timing->fraction_value(-1599), 0);
    XCTAssertEqual(timing->fraction_value(-1600), 1);
    XCTAssertEqual(timing->fraction_value(-47999), 29);
    XCTAssertEqual(timing->fraction_value(-48000), 0);
}

@end

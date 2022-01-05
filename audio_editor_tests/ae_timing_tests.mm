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
    XCTAssertEqual(timing->fragment(), timing_fragment::sample);
}

- (void)test_set_fragment {
    auto const timing = timing::make_shared(48000);

    std::vector<timing_fragment> called;

    auto canceller =
        timing->observe_fragment([&called](timing_fragment const &fragment) { called.emplace_back(fragment); }).sync();

    XCTAssertEqual(called.size(), 1);
    XCTAssertEqual(called.at(0), timing_fragment::sample);
    XCTAssertEqual(timing->fragment(), timing_fragment::sample);
    XCTAssertEqual(timing->unit_sample_count(), 1);

    timing->set_fragment(timing_fragment::milisecond);

    XCTAssertEqual(called.size(), 2);
    XCTAssertEqual(called.at(1), timing_fragment::milisecond);
    XCTAssertEqual(timing->fragment(), timing_fragment::milisecond);
    XCTAssertEqual(timing->unit_sample_count(), 48);

    timing->set_fragment(timing_fragment::frame30);

    XCTAssertEqual(called.size(), 3);
    XCTAssertEqual(called.at(2), timing_fragment::frame30);
    XCTAssertEqual(timing->fragment(), timing_fragment::frame30);
    XCTAssertEqual(timing->unit_sample_count(), 1600);

    canceller->cancel();
}

@end

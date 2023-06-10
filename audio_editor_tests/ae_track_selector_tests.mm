//
//  ae_track_selector_tests.mm
//

#import <XCTest/XCTest.h>
#include <audio_editor_core/ae_track_selector.hpp>

namespace yas::ae::track_selector_test_utils {
struct scrolling_mock final : scrolling_for_track_selector {
    observing::value::holder_ptr<double> const track = observing::value::holder<double>::make_shared(0.0);

    [[nodiscard]] observing::syncable observe_track(std::function<void(double const &)> &&handler) override {
        return track->observe(std::move(handler));
    }
};
}

using namespace yas;
using namespace yas::ae;
using namespace yas::ae::track_selector_test_utils;

@interface ae_track_selector_tests : XCTestCase

@end

@implementation ae_track_selector_tests

- (void)test_current {
    auto const scrolling = std::make_shared<scrolling_mock>();
    auto const selector = std::make_unique<track_selector>(scrolling.get());

    XCTAssertEqual(selector->current(), 0);

    std::vector<track_index_t> received;
    auto const canceller =
        selector->observe_current([&received](track_index_t const &track) { received.emplace_back(track); }).sync();

    XCTAssertEqual(received.size(), 1);
    XCTAssertEqual(received.at(0), 0);

    scrolling->track->set_value(0.01);
    scrolling->track->set_value(0.49);

    XCTAssertEqual(received.size(), 1);

    scrolling->track->set_value(0.5);

    XCTAssertEqual(selector->current(), 1);
    XCTAssertEqual(received.size(), 2);
    XCTAssertEqual(received.at(1), 1);

    scrolling->track->set_value(-0.5);

    XCTAssertEqual(selector->current(), -1);
    XCTAssertEqual(received.size(), 3);
    XCTAssertEqual(received.at(2), -1);
}

@end

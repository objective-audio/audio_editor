//
//  vertical_scrolling_tests.mm
//

#import <XCTest/XCTest.h>
#include <ae-core/project/features/vertical_scrolling.hpp>

using namespace yas;
using namespace yas::ae;

@interface vertical_scrolling_tests : XCTestCase

@end

@implementation vertical_scrolling_tests

- (void)test_track {
    auto const scrolling = std::make_unique<vertical_scrolling>();

    std::vector<track_index_t> received;
    auto const canceller =
        scrolling->observe_track([&received](double const &track) { received.emplace_back(track); }).sync();

    XCTAssertEqual(scrolling->track(), 0);
    XCTAssertEqual(received.size(), 1);
    XCTAssertEqual(received.at(0), 0.0);

    scrolling->set_delta_position(1.0);

    // beginしていなければ反映されない
    XCTAssertEqual(scrolling->track(), 0);
    XCTAssertEqual(received.size(), 1);

    scrolling->begin();

    scrolling->set_delta_position(1.0);

    XCTAssertEqual(scrolling->track(), 1);
    XCTAssertEqual(received.size(), 2);
    XCTAssertEqual(received.at(1), 1);

    scrolling->set_delta_position(1.0);

    XCTAssertEqual(scrolling->track(), 2);
    XCTAssertEqual(received.size(), 3);
    XCTAssertEqual(received.at(2), 2);

    scrolling->end();

    scrolling->set_delta_position(1.0);

    // endしたら反映されない
    XCTAssertEqual(scrolling->track(), 2);
    XCTAssertEqual(received.size(), 3);

    canceller->cancel();
}

@end

//
//  ae_scrolling_tests.mm
//

#import <XCTest/XCTest.h>
#include <audio_editor_core/ae_scrolling.h>

using namespace yas;
using namespace yas::ae;

@interface ae_scrolling_tests : XCTestCase

@end

@implementation ae_scrolling_tests

- (void)test_initial {
    auto const scrolling = scrolling::make_shared();

    XCTAssertFalse(scrolling->is_began());
}

- (void)test_observe {
    auto const scrolling = scrolling::make_shared();

    std::vector<scrolling_event> called;

    auto const canceller =
        scrolling->observe([&called](scrolling_event const &event) { called.emplace_back(event); }).end();

    scrolling->begin();

    XCTAssertEqual(called.size(), 1);
    XCTAssertEqual(called.at(0).state, scrolling_state::began);
    XCTAssertEqual(called.at(0).delta_time, 0.0);

    scrolling->set_delta_time(1.0);

    XCTAssertEqual(called.size(), 2);
    XCTAssertEqual(called.at(1).state, scrolling_state::changed);
    XCTAssertEqual(called.at(1).delta_time, 1.0);

    scrolling->set_delta_time(2.0);

    XCTAssertEqual(called.size(), 3);
    XCTAssertEqual(called.at(2).state, scrolling_state::changed);
    XCTAssertEqual(called.at(2).delta_time, 2.0);

    scrolling->end();

    XCTAssertEqual(called.size(), 4);
    XCTAssertEqual(called.at(3).state, scrolling_state::ended);
    XCTAssertEqual(called.at(3).delta_time, 0.0);

    scrolling->set_delta_time(3.0);
    scrolling->end();

    XCTAssertEqual(called.size(), 4);

    canceller->cancel();
}

@end

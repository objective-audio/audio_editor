//
//  ae_pasteboard_tests.mm
//

#import <XCTest/XCTest.h>

#include <audio_editor_core/ae_pasteboard.h>

using namespace yas;
using namespace yas::ae;

@interface ae_pasteboard_tests : XCTestCase

@end

@implementation ae_pasteboard_tests

- (void)test_data {
    auto const pasteboard = pasteboard::make_shared();

    std::vector<pasteboard_event> called;

    auto const canceller =
        pasteboard->observe_event([&called](pasteboard_event const &event) { called.push_back(event); }).sync();

    XCTAssertEqual(called.size(), 1);
    XCTAssertEqual(called.at(0), pasteboard_event::fetched);

    XCTAssertEqual(pasteboard->data(), "");

    pasteboard->revert_data("test_data");

    XCTAssertEqual(called.size(), 2);
    XCTAssertEqual(called.at(1), pasteboard_event::reverted);

    XCTAssertEqual(pasteboard->data(), "test_data");

    canceller->cancel();
}

- (void)test_file_module {
    auto const pasteboard = pasteboard::make_shared();

    std::vector<pasteboard_event> called;

    auto const canceller =
        pasteboard->observe_event([&called](pasteboard_event const &event) { called.push_back(event); }).sync();

    XCTAssertEqual(called.size(), 1);
    XCTAssertEqual(called.at(0), pasteboard_event::fetched);

    XCTAssertFalse(pasteboard->file_module().has_value());

    pasteboard->set_file_module({.file_frame = 1, .length = 2});

    XCTAssertEqual(called.size(), 2);
    XCTAssertEqual(called.at(1), pasteboard_event::file_module);

    auto const module = pasteboard->file_module();
    XCTAssertTrue(module.has_value());
    XCTAssertEqual(module.value().file_frame, 1);
    XCTAssertEqual(module.value().length, 2);
}

@end

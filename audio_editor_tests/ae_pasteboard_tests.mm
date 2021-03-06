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

    XCTAssertFalse(pasteboard->value().has_value());

    pasting_file_module const module{
        .name = "test-name", .file_name = "test-file-name", .range = {1, 2}, .file_frame = 3};

    pasteboard->revert_value(module);

    XCTAssertEqual(called.size(), 2);
    XCTAssertEqual(called.at(1), pasteboard_event::reverted);

    XCTAssertEqual(pasteboard->value(), pasting_value(module));

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

    pasteboard->set_file_module(
        {.name = "test-name-1", .file_frame = 1, .range = {2, 3}, .file_name = "test-file-name-1"});

    XCTAssertEqual(called.size(), 2);
    XCTAssertEqual(called.at(1), pasteboard_event::file_module);

    auto const module = pasteboard->file_module();
    XCTAssertTrue(module.has_value());
    XCTAssertEqual(module.value().name, "test-name-1");
    XCTAssertEqual(module.value().file_frame, 1);
    XCTAssertEqual(module.value().range, time::range(2, 3));
    XCTAssertEqual(module.value().file_name, "test-file-name-1");
}

@end

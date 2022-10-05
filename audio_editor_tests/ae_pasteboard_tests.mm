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

    pasting_file_module const module{"test-name", 3, {1, 2}, "test-file-name"};

    pasteboard->clear();

    XCTAssertEqual(called.size(), 2);
    XCTAssertEqual(called.at(1), pasteboard_event::cleared);

    XCTAssertFalse(pasteboard->value().has_value());

    canceller->cancel();
}

- (void)test_file_module {
    auto const pasteboard = pasteboard::make_shared();

    std::vector<pasteboard_event> called;

    auto const canceller =
        pasteboard->observe_event([&called](pasteboard_event const &event) { called.push_back(event); }).sync();

    XCTAssertEqual(called.size(), 1);
    XCTAssertEqual(called.at(0), pasteboard_event::fetched);

    XCTAssertEqual(pasteboard->file_modules().size(), 0);

    identifier const module_id_0;
    identifier const module_id_1;

    pasteboard->set_file_modules({{module_id_0, {"test-name-1", 1, {2, 3}, "test-file-name-1"}},
                                  {module_id_1, {"test-name-2", 4, {5, 6}, "test-file-name-2"}}});

    XCTAssertEqual(called.size(), 2);
    XCTAssertEqual(called.at(1), pasteboard_event::file_modules);

    auto const modules = pasteboard->file_modules();
    XCTAssertEqual(modules.size(), 2);
    XCTAssertEqual(modules.at(0).identifier, module_id_0);
    XCTAssertEqual(modules.at(0).value.name, "test-name-1");
    XCTAssertEqual(modules.at(0).value.file_frame, 1);
    XCTAssertEqual(modules.at(0).value.range, time::range(2, 3));
    XCTAssertEqual(modules.at(0).value.file_name, "test-file-name-1");
    XCTAssertEqual(modules.at(1).identifier, module_id_1);
    XCTAssertEqual(modules.at(1).value.name, "test-name-2");
    XCTAssertEqual(modules.at(1).value.file_frame, 4);
    XCTAssertEqual(modules.at(1).value.range, time::range(5, 6));
    XCTAssertEqual(modules.at(1).value.file_name, "test-file-name-2");
}

@end

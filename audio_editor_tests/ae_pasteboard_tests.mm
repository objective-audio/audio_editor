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

- (void)test_module {
    auto const pasteboard = std::make_shared<ae::pasteboard>();

    std::vector<pasteboard_event> called;

    auto const canceller =
        pasteboard->observe_event([&called](pasteboard_event const &event) { called.push_back(event); }).sync();

    XCTAssertEqual(called.size(), 1);
    XCTAssertEqual(called.at(0), pasteboard_event::fetched);

    XCTAssertEqual(pasteboard->modules().size(), 0);

    identifier const module_id_0;
    identifier const module_id_1;

    pasteboard->set_modules({{module_id_0, {"test-name-1", 1, {2, 3}, "test-file-name-1"}},
                             {module_id_1, {"test-name-2", 4, {5, 6}, "test-file-name-2"}}});

    XCTAssertEqual(called.size(), 2);
    XCTAssertEqual(called.at(1), pasteboard_event::modules_replaced);

    auto const &modules = pasteboard->modules();
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

    pasteboard->clear();

    XCTAssertEqual(called.size(), 3);
    XCTAssertEqual(called.at(2), pasteboard_event::modules_cleared);

    XCTAssertEqual(pasteboard->modules().size(), 0);
}

- (void)test_marker {
    auto const pasteboard = std::make_shared<ae::pasteboard>();

    std::vector<pasteboard_event> called;

    auto const canceller =
        pasteboard->observe_event([&called](pasteboard_event const &event) { called.push_back(event); }).sync();

    XCTAssertEqual(called.size(), 1);
    XCTAssertEqual(called.at(0), pasteboard_event::fetched);

    XCTAssertEqual(pasteboard->markers().size(), 0);

    identifier const marker_id_0;
    identifier const marker_id_1;

    pasteboard->set_markers({{marker_id_0, {1, "test-marker-name-1"}}, {marker_id_1, {2, "test-marker-name-2"}}});

    XCTAssertEqual(called.size(), 2);
    XCTAssertEqual(called.at(1), pasteboard_event::markers_replaced);

    auto const &markers = pasteboard->markers();
    XCTAssertEqual(markers.size(), 2);
    XCTAssertEqual(markers.at(0).identifier, marker_id_0);
    XCTAssertEqual(markers.at(0).value.frame, 1);
    XCTAssertEqual(markers.at(0).value.name, "test-marker-name-1");
    XCTAssertEqual(markers.at(1).identifier, marker_id_1);
    XCTAssertEqual(markers.at(1).value.frame, 2);
    XCTAssertEqual(markers.at(1).value.name, "test-marker-name-2");

    pasteboard->clear();

    XCTAssertEqual(called.size(), 3);
    XCTAssertEqual(called.at(2), pasteboard_event::markers_cleared);

    XCTAssertEqual(pasteboard->markers().size(), 0);
}

@end

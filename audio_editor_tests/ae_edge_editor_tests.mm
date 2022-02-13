//
//  ae_edge_editor_tests.mm
//

#import <XCTest/XCTest.h>
#include <audio_editor_core/ae_edge_editor.h>

using namespace yas;
using namespace yas::ae;

@interface ae_edge_editor_tests : XCTestCase

@end

@implementation ae_edge_editor_tests

- (void)test_initial {
    auto const editor = edge_editor::make_shared();

    XCTAssertEqual(editor->edge(), ae::edge::zero());
}

- (void)test_set_edge {
    auto const editor = edge_editor::make_shared();

    std::vector<edge_editor_event> called;

    auto const canceller =
        editor->observe_event([&called](edge_editor_event const &event) { called.push_back(event); }).sync();

    XCTAssertEqual(called.size(), 1);
    XCTAssertEqual(called.at(0).type, edge_editor_event_type::fetched);
    XCTAssertEqual(called.at(0).edge, ae::edge::zero());

    editor->set_edge(ae::edge::zero());

    XCTAssertEqual(called.size(), 1);

    editor->set_edge({.begin_frame = 1, .end_frame = 2});

    XCTAssertEqual(called.size(), 2);
    XCTAssertEqual(called.at(1).type, edge_editor_event_type::updated);
    XCTAssertEqual(called.at(1).edge, (ae::edge{.begin_frame = 1, .end_frame = 2}));

    XCTAssertThrows(editor->set_edge({.begin_frame = 1, .end_frame = 0}));

    canceller->cancel();
}

- (void)test_set_begin_frame {
    auto const editor = edge_editor::make_shared();
    editor->set_edge({.begin_frame = 0, .end_frame = 2});

    std::vector<edge_editor_event> called;

    auto const canceller =
        editor->observe_event([&called](edge_editor_event const &event) { called.push_back(event); }).sync();

    XCTAssertEqual(called.size(), 1);

    editor->set_begin_frame(0);

    XCTAssertEqual(called.size(), 1);

    editor->set_begin_frame(1);

    XCTAssertEqual(editor->edge(), (ae::edge{.begin_frame = 1, .end_frame = 2}));
    XCTAssertEqual(called.size(), 2);
    XCTAssertEqual(called.at(1).type, edge_editor_event_type::updated);
    XCTAssertEqual(called.at(1).edge, (ae::edge{.begin_frame = 1, .end_frame = 2}));

    editor->set_begin_frame(3);

    XCTAssertEqual(editor->edge(), (ae::edge{.begin_frame = 3, .end_frame = 3}));
    XCTAssertEqual(called.size(), 3);
    XCTAssertEqual(called.at(2).type, edge_editor_event_type::updated);
    XCTAssertEqual(called.at(2).edge, (ae::edge{.begin_frame = 3, .end_frame = 3}));

    canceller->cancel();
}

- (void)test_set_end_frame {
    auto const editor = edge_editor::make_shared();
    editor->set_edge({.begin_frame = 1, .end_frame = 2});

    std::vector<edge_editor_event> called;

    auto const canceller =
        editor->observe_event([&called](edge_editor_event const &event) { called.push_back(event); }).sync();

    XCTAssertEqual(called.size(), 1);

    editor->set_end_frame(2);

    XCTAssertEqual(called.size(), 1);

    editor->set_end_frame(3);

    XCTAssertEqual(editor->edge(), (ae::edge{.begin_frame = 1, .end_frame = 3}));
    XCTAssertEqual(called.size(), 2);
    XCTAssertEqual(called.at(1).type, edge_editor_event_type::updated);
    XCTAssertEqual(called.at(1).edge, (ae::edge{.begin_frame = 1, .end_frame = 3}));

    editor->set_end_frame(0);

    XCTAssertEqual(editor->edge(), (ae::edge{.begin_frame = 0, .end_frame = 0}));
    XCTAssertEqual(called.size(), 3);
    XCTAssertEqual(called.at(2).type, edge_editor_event_type::updated);
    XCTAssertEqual(called.at(2).edge, (ae::edge{.begin_frame = 0, .end_frame = 0}));

    canceller->cancel();
}

- (void)test_revert_edge {
    auto const editor = edge_editor::make_shared();
    editor->set_edge({.begin_frame = 0, .end_frame = 2});

    std::vector<edge_editor_event> called;

    auto const canceller =
        editor->observe_event([&called](edge_editor_event const &event) { called.push_back(event); }).sync();

    XCTAssertEqual(called.size(), 1);

    editor->revert_edge({.begin_frame = 0, .end_frame = 2});

    XCTAssertEqual(called.size(), 1);

    editor->revert_edge({.begin_frame = 1, .end_frame = 3});

    XCTAssertEqual(editor->edge(), (ae::edge{.begin_frame = 1, .end_frame = 3}));
    XCTAssertEqual(called.size(), 2);
    XCTAssertEqual(called.at(1).type, edge_editor_event_type::reverted);
    XCTAssertEqual(called.at(1).edge, (ae::edge{.begin_frame = 1, .end_frame = 3}));
}

@end

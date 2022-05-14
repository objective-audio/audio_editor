//
//  ae_edge_holder_tests.mm
//

#import <XCTest/XCTest.h>
#include <audio_editor_core/ae_edge_holder.h>

using namespace yas;
using namespace yas::ae;

@interface ae_edge_holder_tests : XCTestCase

@end

@implementation ae_edge_holder_tests

- (void)test_initial {
    auto const editor = edge_holder::make_shared();

    XCTAssertEqual(editor->edge(), ae::edge::zero());
}

- (void)test_set_edge {
    auto const holder = edge_holder::make_shared();

    std::vector<edge_holder_event> called;

    auto const canceller =
        holder->observe_event([&called](edge_holder_event const &event) { called.push_back(event); }).sync();

    XCTAssertEqual(called.size(), 1);
    XCTAssertEqual(called.at(0).type, edge_holder_event_type::fetched);
    XCTAssertEqual(called.at(0).edge, ae::edge::zero());

    holder->set_edge(ae::edge::zero());

    XCTAssertEqual(called.size(), 1);

    holder->set_edge({.begin_frame = 1, .end_frame = 2});

    XCTAssertEqual(called.size(), 2);
    XCTAssertEqual(called.at(1).type, edge_holder_event_type::updated);
    XCTAssertEqual(called.at(1).edge, (ae::edge{.begin_frame = 1, .end_frame = 2}));

    XCTAssertThrows(holder->set_edge({.begin_frame = 1, .end_frame = 0}));

    canceller->cancel();
}

- (void)test_set_begin_frame {
    auto const holder = edge_holder::make_shared();
    holder->set_edge({.begin_frame = 0, .end_frame = 2});

    std::vector<edge_holder_event> called;

    auto const canceller =
        holder->observe_event([&called](edge_holder_event const &event) { called.push_back(event); }).sync();

    XCTAssertEqual(called.size(), 1);

    holder->set_begin_frame(0);

    XCTAssertEqual(called.size(), 1);

    holder->set_begin_frame(1);

    XCTAssertEqual(holder->edge(), (ae::edge{.begin_frame = 1, .end_frame = 2}));
    XCTAssertEqual(called.size(), 2);
    XCTAssertEqual(called.at(1).type, edge_holder_event_type::updated);
    XCTAssertEqual(called.at(1).edge, (ae::edge{.begin_frame = 1, .end_frame = 2}));

    holder->set_begin_frame(3);

    XCTAssertEqual(holder->edge(), (ae::edge{.begin_frame = 3, .end_frame = 3}));
    XCTAssertEqual(called.size(), 3);
    XCTAssertEqual(called.at(2).type, edge_holder_event_type::updated);
    XCTAssertEqual(called.at(2).edge, (ae::edge{.begin_frame = 3, .end_frame = 3}));

    canceller->cancel();
}

- (void)test_set_end_frame {
    auto const holder = edge_holder::make_shared();
    holder->set_edge({.begin_frame = 1, .end_frame = 2});

    std::vector<edge_holder_event> called;

    auto const canceller =
        holder->observe_event([&called](edge_holder_event const &event) { called.push_back(event); }).sync();

    XCTAssertEqual(called.size(), 1);

    holder->set_end_frame(2);

    XCTAssertEqual(called.size(), 1);

    holder->set_end_frame(3);

    XCTAssertEqual(holder->edge(), (ae::edge{.begin_frame = 1, .end_frame = 3}));
    XCTAssertEqual(called.size(), 2);
    XCTAssertEqual(called.at(1).type, edge_holder_event_type::updated);
    XCTAssertEqual(called.at(1).edge, (ae::edge{.begin_frame = 1, .end_frame = 3}));

    holder->set_end_frame(0);

    XCTAssertEqual(holder->edge(), (ae::edge{.begin_frame = 0, .end_frame = 0}));
    XCTAssertEqual(called.size(), 3);
    XCTAssertEqual(called.at(2).type, edge_holder_event_type::updated);
    XCTAssertEqual(called.at(2).edge, (ae::edge{.begin_frame = 0, .end_frame = 0}));

    canceller->cancel();
}

- (void)test_revert_edge {
    auto const holder = edge_holder::make_shared();
    holder->set_edge({.begin_frame = 0, .end_frame = 2});

    std::vector<edge_holder_event> called;

    auto const canceller =
        holder->observe_event([&called](edge_holder_event const &event) { called.push_back(event); }).sync();

    XCTAssertEqual(called.size(), 1);

    holder->revert_edge({.begin_frame = 0, .end_frame = 2});

    XCTAssertEqual(called.size(), 1);

    holder->revert_edge({.begin_frame = 1, .end_frame = 3});

    XCTAssertEqual(holder->edge(), (ae::edge{.begin_frame = 1, .end_frame = 3}));
    XCTAssertEqual(called.size(), 2);
    XCTAssertEqual(called.at(1).type, edge_holder_event_type::reverted);
    XCTAssertEqual(called.at(1).edge, (ae::edge{.begin_frame = 1, .end_frame = 3}));
}

@end

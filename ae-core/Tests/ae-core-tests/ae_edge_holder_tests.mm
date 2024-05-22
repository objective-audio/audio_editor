//
//  ae_edge_holder_tests.mm
//

#import <XCTest/XCTest.h>
#include <ae-core/project_editing/features/ae_edge_holder.h>

using namespace yas;
using namespace yas::ae;

namespace yas::ae::edge_holder_test_utils {
struct database_mock : database_for_edge_holder {
    std::vector<ae::edge> called_edges;

    void set_edge(ae::edge const &edge) override {
        called_edges.emplace_back(edge);
    }
};
}  // namespace yas::ae::edge_holder_test_utils

@interface ae_edge_holder_tests : XCTestCase

@end

@implementation ae_edge_holder_tests

- (void)test_initial {
    auto const database = std::make_shared<edge_holder_test_utils::database_mock>();
    auto const holder = std::make_shared<ae::edge_holder>(database.get());

    XCTAssertEqual(holder->edge(), ae::edge::zero());
}

- (void)test_set_edge {
    auto const database = std::make_shared<edge_holder_test_utils::database_mock>();
    auto const holder = std::make_shared<ae::edge_holder>(database.get());

    std::vector<edge_holder_event> called;

    auto const canceller =
        holder->observe_event([&called](edge_holder_event const &event) { called.push_back(event); }).sync();

    XCTAssertEqual(called.size(), 1);
    XCTAssertEqual(called.at(0).type, edge_holder_event_type::fetched);
    XCTAssertEqual(called.at(0).edge, ae::edge::zero());

    holder->set_edge(ae::edge::zero());

    // 値が同じなら通知も保存もされない
    XCTAssertEqual(called.size(), 1);
    XCTAssertEqual(database->called_edges.size(), 0);

    holder->set_edge({1, 2});

    XCTAssertEqual(called.size(), 2);
    XCTAssertEqual(called.at(1).type, edge_holder_event_type::updated);
    XCTAssertEqual(called.at(1).edge, (ae::edge{1, 2}));

    XCTAssertThrows(holder->set_edge({1, 0}));
    XCTAssertEqual(database->called_edges.size(), 1);
    XCTAssertEqual(database->called_edges.at(0), (ae::edge{1, 2}));

    canceller->cancel();
}

- (void)test_set_begin_frame {
    auto const database = std::make_shared<edge_holder_test_utils::database_mock>();
    auto const holder = std::make_shared<ae::edge_holder>(database.get());
    holder->set_edge({0, 2});
    database->called_edges.clear();

    std::vector<edge_holder_event> called;

    auto const canceller =
        holder->observe_event([&called](edge_holder_event const &event) { called.push_back(event); }).sync();

    XCTAssertEqual(called.size(), 1);
    XCTAssertEqual(database->called_edges.size(), 0);

    holder->set_begin_frame(0);

    XCTAssertEqual(called.size(), 1);
    XCTAssertEqual(database->called_edges.size(), 0);

    holder->set_begin_frame(1);

    XCTAssertEqual(holder->edge(), (ae::edge{1, 2}));
    XCTAssertEqual(called.size(), 2);
    XCTAssertEqual(called.at(1).type, edge_holder_event_type::updated);
    XCTAssertEqual(called.at(1).edge, (ae::edge{1, 2}));
    XCTAssertEqual(database->called_edges.size(), 1);
    XCTAssertEqual(database->called_edges.at(0), (ae::edge{1, 2}));

    holder->set_begin_frame(3);

    XCTAssertEqual(holder->edge(), (ae::edge{3, 3}));
    XCTAssertEqual(called.size(), 3);
    XCTAssertEqual(called.at(2).type, edge_holder_event_type::updated);
    XCTAssertEqual(called.at(2).edge, (ae::edge{3, 3}));
    XCTAssertEqual(database->called_edges.size(), 2);
    XCTAssertEqual(database->called_edges.at(1), (ae::edge{3, 3}));

    canceller->cancel();
}

- (void)test_set_end_frame {
    auto const database = std::make_shared<edge_holder_test_utils::database_mock>();
    auto const holder = std::make_shared<ae::edge_holder>(database.get());
    holder->set_edge({1, 2});
    database->called_edges.clear();

    std::vector<edge_holder_event> called;

    auto const canceller =
        holder->observe_event([&called](edge_holder_event const &event) { called.push_back(event); }).sync();

    XCTAssertEqual(called.size(), 1);
    XCTAssertEqual(database->called_edges.size(), 0);

    holder->set_end_frame(2);

    XCTAssertEqual(called.size(), 1);
    XCTAssertEqual(database->called_edges.size(), 0);

    holder->set_end_frame(3);

    XCTAssertEqual(holder->edge(), (ae::edge{1, 3}));
    XCTAssertEqual(called.size(), 2);
    XCTAssertEqual(called.at(1).type, edge_holder_event_type::updated);
    XCTAssertEqual(called.at(1).edge, (ae::edge{1, 3}));
    XCTAssertEqual(database->called_edges.size(), 1);
    XCTAssertEqual(database->called_edges.at(0), (ae::edge{1, 3}));

    holder->set_end_frame(0);

    XCTAssertEqual(holder->edge(), (ae::edge{0, 0}));
    XCTAssertEqual(called.size(), 3);
    XCTAssertEqual(called.at(2).type, edge_holder_event_type::updated);
    XCTAssertEqual(called.at(2).edge, (ae::edge{0, 0}));
    XCTAssertEqual(database->called_edges.size(), 2);
    XCTAssertEqual(database->called_edges.at(1), (ae::edge{0, 0}));

    canceller->cancel();
}

- (void)test_revert_edge {
    auto const database = std::make_shared<edge_holder_test_utils::database_mock>();
    auto const holder = std::make_shared<ae::edge_holder>(database.get());
    holder->set_edge({0, 2});
    database->called_edges.clear();

    std::vector<edge_holder_event> called;

    auto const canceller =
        holder->observe_event([&called](edge_holder_event const &event) { called.push_back(event); }).sync();

    XCTAssertEqual(called.size(), 1);
    XCTAssertEqual(database->called_edges.size(), 0);

    holder->revert_edge({0, 2});

    XCTAssertEqual(called.size(), 1);
    XCTAssertEqual(database->called_edges.size(), 0);

    holder->revert_edge({1, 3});

    XCTAssertEqual(holder->edge(), (ae::edge{1, 3}));
    XCTAssertEqual(called.size(), 2);
    XCTAssertEqual(called.at(1).type, edge_holder_event_type::reverted);
    XCTAssertEqual(called.at(1).edge, (ae::edge{1, 3}));
    XCTAssertEqual(database->called_edges.size(), 0, @"revertでは保存されない");
}

@end

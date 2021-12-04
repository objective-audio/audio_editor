//
//  ae_marker_pool_tests.mm
//

#import <XCTest/XCTest.h>
#import <audio_editor_core/audio_editor_core_umbrella.h>

using namespace yas;
using namespace yas::ae;

@interface ae_marker_pool_tests : XCTestCase

@end

@implementation ae_marker_pool_tests

- (void)test_initial {
    auto const pool = marker_pool::make_shared();

    XCTAssertEqual(pool->markers().size(), 0);
}

- (void)test_revert {
    auto const pool = marker_pool::make_shared();

    identifier const id_0;
    identifier const id_1;
    identifier const id_m1;

    pool->revert_markers(
        {{.identifier = id_0, .frame = 0}, {.identifier = id_1, .frame = 1}, {.identifier = id_m1, .frame = -1}});

    XCTAssertEqual(pool->markers().size(), 3);

    auto iterator = pool->markers().begin();
    XCTAssertEqual(iterator->first, -1);
    XCTAssertEqual(iterator->second.identifier, id_m1);
    ++iterator;
    XCTAssertEqual(iterator->first, 0);
    XCTAssertEqual(iterator->second.identifier, id_0);
    ++iterator;
    XCTAssertEqual(iterator->first, 1);
    XCTAssertEqual(iterator->second.identifier, id_1);
}

- (void)test_insert {
    auto const pool = marker_pool::make_shared();

    identifier const id_0;
    identifier const id_1;

    pool->insert_marker({.identifier = id_0, .frame = 0});

    XCTAssertEqual(pool->markers().size(), 1);
    XCTAssertEqual(pool->markers().at(0).identifier, id_0);

    pool->insert_marker({.identifier = id_1, .frame = 1});

    XCTAssertEqual(pool->markers().size(), 2);
    XCTAssertEqual(pool->markers().at(1).identifier, id_1);
}

- (void)test_erase_at {
    auto const pool = marker_pool::make_shared();

    identifier const id_0;
    identifier const id_1;
    identifier const id_m1;

    pool->revert_markers(
        {{.identifier = id_0, .frame = 0}, {.identifier = id_1, .frame = 1}, {.identifier = id_m1, .frame = -1}});

    pool->erase_at(0);

    XCTAssertEqual(pool->markers().size(), 2);
    XCTAssertEqual(pool->markers().at(-1).identifier, id_m1);
    XCTAssertEqual(pool->markers().at(1).identifier, id_1);
}

- (void)test_erase_range {
    auto const pool = marker_pool::make_shared();

    identifier const id_0;
    identifier const id_1;
    identifier const id_2;
    identifier const id_3;

    pool->revert_markers({{.identifier = id_0, .frame = 0},
                          {.identifier = id_1, .frame = 1},
                          {.identifier = id_2, .frame = 2},
                          {.identifier = id_3, .frame = 3}});

    pool->erase_range({1, 2});

    XCTAssertEqual(pool->markers().size(), 2);
    XCTAssertEqual(pool->markers().at(0).identifier, id_0);
    XCTAssertEqual(pool->markers().at(3).identifier, id_3);
}

- (void)test_move_marker_at {
    auto const pool = marker_pool::make_shared();

    identifier const id_0;
    identifier const id_1;
    identifier const id_m1;

    pool->revert_markers(
        {{.identifier = id_m1, .frame = -1}, {.identifier = id_0, .frame = 0}, {.identifier = id_1, .frame = 1}});

    pool->move_at(0, 2);

    XCTAssertEqual(pool->markers().size(), 3);
    XCTAssertEqual(pool->markers().at(-1).identifier, id_m1);
    XCTAssertEqual(pool->markers().at(1).identifier, id_1);
    // moveしたらidentifierが変わる
    XCTAssertNotEqual(pool->markers().at(2).identifier, id_0);
    XCTAssertEqual(pool->markers().at(2).frame, 2);
}

- (void)test_move_offset_from {
    auto const pool = marker_pool::make_shared();

    identifier const id_m1;
    identifier const id_0;
    identifier const id_1;

    pool->revert_markers(
        {{.identifier = id_m1, .frame = -1}, {.identifier = id_0, .frame = 0}, {.identifier = id_1, .frame = 1}});

    pool->move_offset_from(0, 2);

    XCTAssertEqual(pool->markers().size(), 3);
    XCTAssertEqual(pool->markers().at(-1).identifier, id_m1);
    // moveしたらidentifierが変わる
    XCTAssertNotEqual(pool->markers().at(2).identifier, id_0);
    XCTAssertEqual(pool->markers().at(2).frame, 2);
    XCTAssertNotEqual(pool->markers().at(3).identifier, id_1);
    XCTAssertEqual(pool->markers().at(3).frame, 3);
}

- (void)test_observe {
    auto const pool = marker_pool::make_shared();

    identifier const id_0;
    identifier const id_1;
    identifier const id_m1;

    pool->revert_markers({{.identifier = id_0, .frame = 0}, {.identifier = id_1, .frame = 1}});

    std::vector<marker_pool_event> called;

    auto canceller = pool->observe_event([&called](auto const &event) { called.emplace_back(event); }).sync();

    XCTAssertEqual(called.size(), 1);
    XCTAssertEqual(called.at(0).type, marker_pool_event_type::any);
    XCTAssertEqual(called.at(0).marker, std::nullopt);

    pool->insert_marker({.identifier = id_m1, .frame = -1});

    XCTAssertEqual(called.size(), 2);
    XCTAssertEqual(called.at(1).type, marker_pool_event_type::inserted);
    XCTAssertEqual(called.at(1).marker.value().identifier, id_m1);

    pool->erase_at(0);

    XCTAssertEqual(called.size(), 3);
    XCTAssertEqual(called.at(2).type, marker_pool_event_type::erased);
    XCTAssertEqual(called.at(2).marker.value().identifier, id_0);

    canceller->cancel();
}

@end

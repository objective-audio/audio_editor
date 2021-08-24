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

- (void)test_replace {
    auto const pool = marker_pool::make_shared();

    pool->replace_markers({{.frame = 0}, {.frame = 1}, {.frame = -1}});

    XCTAssertEqual(pool->markers().size(), 3);

    auto iterator = pool->markers().begin();
    XCTAssertEqual(iterator->first, -1);
    XCTAssertEqual(iterator->second, (marker{.frame = -1}));
    ++iterator;
    XCTAssertEqual(iterator->first, 0);
    XCTAssertEqual(iterator->second, (marker{.frame = 0}));
    ++iterator;
    XCTAssertEqual(iterator->first, 1);
    XCTAssertEqual(iterator->second, (marker{.frame = 1}));
}

- (void)test_insert {
    auto const pool = marker_pool::make_shared();

    pool->insert_marker({.frame = 0});

    XCTAssertEqual(pool->markers().size(), 1);
    XCTAssertEqual(pool->markers().at(0), (marker{.frame = 0}));

    pool->insert_marker({.frame = 1});

    XCTAssertEqual(pool->markers().size(), 2);
    XCTAssertEqual(pool->markers().at(1), (marker{.frame = 1}));
}

- (void)test_erase_at {
    auto const pool = marker_pool::make_shared();

    pool->replace_markers({{.frame = 0}, {.frame = 1}, {.frame = -1}});

    pool->erase_at(0);

    XCTAssertEqual(pool->markers().size(), 2);
    XCTAssertEqual(pool->markers().at(-1), (marker{.frame = -1}));
    XCTAssertEqual(pool->markers().at(1), (marker{.frame = 1}));
}

- (void)test_observe {
    auto const pool = marker_pool::make_shared();

    pool->replace_markers({{.frame = 0}, {.frame = 1}});

    std::vector<marker_pool_event> called;

    auto canceller = pool->observe_event([&called](auto const &event) { called.emplace_back(event); }).sync();

    XCTAssertEqual(called.size(), 1);
    XCTAssertEqual(called.at(0).type, marker_pool_event_type::any);
    XCTAssertEqual(called.at(0).marker, std::nullopt);

    pool->insert_marker({.frame = -1});

    XCTAssertEqual(called.size(), 2);
    XCTAssertEqual(called.at(1).type, marker_pool_event_type::inserted);
    XCTAssertEqual(called.at(1).marker, (marker{.frame = -1}));

    pool->erase_at(0);

    XCTAssertEqual(called.size(), 3);
    XCTAssertEqual(called.at(2).type, marker_pool_event_type::erased);
    XCTAssertEqual(called.at(2).marker, (marker{.frame = 0}));

    canceller->cancel();
}

@end

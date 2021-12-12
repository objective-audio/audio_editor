//
//  ae_module_location_pool_tests.mm
//

#import <XCTest/XCTest.h>
#include <audio_editor_core/ae_module_location_pool.h>

using namespace yas;
using namespace yas::ae;

@interface ae_module_location_pool_tests : XCTestCase

@end

@implementation ae_module_location_pool_tests

- (void)test_initial {
    auto const pool = module_location_pool::make_shared();

    XCTAssertEqual(pool->elements().size(), 0);
}

- (void)test_replace_all {
    auto const pool = module_location_pool::make_shared();

    std::vector<module_location_pool_event> called;

    auto canceller =
        pool->observe_event([&called](module_location_pool_event const &event) { called.emplace_back(event); }).sync();

    XCTAssertEqual(called.size(), 1);
    XCTAssertEqual(called.at(0).type, module_location_pool_event_type::fetched);

    identifier id_0;
    identifier id_1;
    identifier id_2;

    {
        pool->replace_all({{.identifier = id_0}, {.identifier = id_1}, {.identifier = id_2}});

        auto const locations = pool->elements();
        XCTAssertEqual(locations.size(), 3);
        XCTAssertEqual(locations.at(0).value().identifier, id_0);
        XCTAssertEqual(locations.at(1).value().identifier, id_1);
        XCTAssertEqual(locations.at(2).value().identifier, id_2);

        XCTAssertEqual(called.size(), 2);
        XCTAssertEqual(called.at(1).type, module_location_pool_event_type::replaced);
    }

    identifier id_3;
    identifier id_4;

    {
        pool->replace_all({{.identifier = id_3}, {.identifier = id_4}});

        auto const locations = pool->elements();
        XCTAssertEqual(locations.size(), 2);
        XCTAssertEqual(locations.at(0).value().identifier, id_3);
        XCTAssertEqual(locations.at(1).value().identifier, id_4);

        XCTAssertEqual(called.size(), 3);
        XCTAssertEqual(called.at(2).type, module_location_pool_event_type::replaced);
    }

    canceller->cancel();
}

- (void)test_update_all {
    auto const pool = module_location_pool::make_shared();
    std::vector<module_location_pool_event> called;

    auto canceller =
        pool->observe_event([&called](module_location_pool_event const &event) { called.emplace_back(event); }).sync();

    XCTAssertEqual(called.size(), 1);
    XCTAssertEqual(called.at(0).type, module_location_pool_event_type::fetched);

    identifier id_0;
    identifier id_1;
    identifier id_2;
    identifier id_3;
    identifier id_4;
    identifier id_5;

    {
        pool->update_all({{.identifier = id_0}, {.identifier = id_2}});

        auto const locations = pool->elements();
        XCTAssertEqual(locations.size(), 2);
        XCTAssertEqual(locations.at(0).value().identifier, id_0);
        XCTAssertEqual(locations.at(1).value().identifier, id_2);

        XCTAssertEqual(called.size(), 2);
        XCTAssertEqual(called.at(1).type, module_location_pool_event_type::updated);
        XCTAssertEqual(called.at(1).inserted.size(), 2);
        XCTAssertEqual(called.at(1).inserted.at(0).first, 0);
        XCTAssertEqual(called.at(1).inserted.at(0).second.identifier, id_0);
        XCTAssertEqual(called.at(1).inserted.at(1).first, 1);
        XCTAssertEqual(called.at(1).inserted.at(1).second.identifier, id_2);
        XCTAssertEqual(called.at(1).erased.size(), 0);
    }

    {
        pool->update_all({{.identifier = id_0}, {.identifier = id_1}, {.identifier = id_2}});

        auto const locations = pool->elements();
        XCTAssertEqual(locations.size(), 3);
        XCTAssertEqual(locations.at(0).value().identifier, id_0);
        XCTAssertEqual(locations.at(1).value().identifier, id_2);
        XCTAssertEqual(locations.at(2).value().identifier, id_1);

        XCTAssertEqual(called.size(), 3);
        XCTAssertEqual(called.at(2).type, module_location_pool_event_type::updated);
        XCTAssertEqual(called.at(2).inserted.size(), 1);
        XCTAssertEqual(called.at(2).inserted.at(0).first, 2);
        XCTAssertEqual(called.at(2).inserted.at(0).second.identifier, id_1);
        XCTAssertEqual(called.at(2).erased.size(), 0);
    }

    {
        pool->update_all({{.identifier = id_1}, {.identifier = id_2}});

        auto const locations = pool->elements();
        XCTAssertEqual(locations.size(), 3);
        XCTAssertFalse(locations.at(0).has_value());
        XCTAssertEqual(locations.at(1).value().identifier, id_2);
        XCTAssertEqual(locations.at(2).value().identifier, id_1);

        XCTAssertEqual(called.size(), 4);
        XCTAssertEqual(called.at(3).type, module_location_pool_event_type::updated);
        XCTAssertEqual(called.at(3).inserted.size(), 0);
        XCTAssertEqual(called.at(3).erased.size(), 1);
        XCTAssertEqual(called.at(3).erased.at(0).first, 0);
        XCTAssertEqual(called.at(3).erased.at(0).second.identifier, id_0);
    }

    {
        pool->update_all({{.identifier = id_1}, {.identifier = id_2}, {.identifier = id_3}, {.identifier = id_4}});
        auto const locations = pool->elements();
        XCTAssertEqual(locations.size(), 4);
        XCTAssertEqual(locations.at(0).value().identifier, id_3);
        XCTAssertEqual(locations.at(1).value().identifier, id_2);
        XCTAssertEqual(locations.at(2).value().identifier, id_1);
        XCTAssertEqual(locations.at(3).value().identifier, id_4);

        XCTAssertEqual(called.size(), 5);
        XCTAssertEqual(called.at(4).type, module_location_pool_event_type::updated);
        XCTAssertEqual(called.at(4).inserted.size(), 2);
        XCTAssertEqual(called.at(4).inserted.at(0).first, 0);
        XCTAssertEqual(called.at(4).inserted.at(0).second.identifier, id_3);
        XCTAssertEqual(called.at(4).inserted.at(1).first, 3);
        XCTAssertEqual(called.at(4).inserted.at(1).second.identifier, id_4);
        XCTAssertEqual(called.at(4).erased.size(), 0);
    }

    {
        pool->update_all({{.identifier = id_1}, {.identifier = id_4}, {.identifier = id_5}});
        auto const locations = pool->elements();
        XCTAssertEqual(locations.size(), 4);
        XCTAssertEqual(locations.at(0).value().identifier, id_5);
        XCTAssertFalse(locations.at(1).has_value());
        XCTAssertEqual(locations.at(2).value().identifier, id_1);
        XCTAssertEqual(locations.at(3).value().identifier, id_4);

        XCTAssertEqual(called.size(), 6);
        XCTAssertEqual(called.at(5).type, module_location_pool_event_type::updated);
        XCTAssertEqual(called.at(5).inserted.size(), 1);
        XCTAssertEqual(called.at(5).inserted.at(0).first, 0);
        XCTAssertEqual(called.at(5).inserted.at(0).second.identifier, id_5);
        XCTAssertEqual(called.at(5).erased.size(), 2);
        XCTAssertEqual(called.at(5).erased.at(0).first, 0);
        XCTAssertEqual(called.at(5).erased.at(0).second.identifier, id_3);
        XCTAssertEqual(called.at(5).erased.at(1).first, 1);
        XCTAssertEqual(called.at(5).erased.at(1).second.identifier, id_2);
    }

    canceller->cancel();
}

- (void)test_erase {
    auto const pool = module_location_pool::make_shared();
    std::vector<module_location_pool_event> called;

    identifier id_0;
    identifier id_1;
    identifier id_2;
    identifier id_3;

    pool->replace_all({{.identifier = id_0}, {.identifier = id_1}, {.identifier = id_2}});

    auto canceller =
        pool->observe_event([&called](module_location_pool_event const &event) { called.emplace_back(event); }).sync();

    XCTAssertEqual(called.size(), 1);
    XCTAssertEqual(called.at(0).type, module_location_pool_event_type::fetched);

    {
        pool->erase(id_1);

        auto const &locations = pool->elements();
        XCTAssertEqual(locations.size(), 3);
        XCTAssertEqual(locations.at(0).value().identifier, id_0);
        XCTAssertFalse(locations.at(1).has_value());
        XCTAssertEqual(locations.at(2).value().identifier, id_2);
        XCTAssertEqual(called.size(), 2);
        XCTAssertEqual(called.at(1).type, module_location_pool_event_type::updated);
        XCTAssertEqual(called.at(1).inserted.size(), 0);
        XCTAssertEqual(called.at(1).erased.size(), 1);
        XCTAssertEqual(called.at(1).erased.at(0).first, 1);
        XCTAssertEqual(called.at(1).erased.at(0).second.identifier, id_1);
    }

    {
        pool->erase(id_3);

        XCTAssertEqual(called.size(), 2);
    }

    canceller->cancel();
}

- (void)test_insert {
    auto const pool = module_location_pool::make_shared();
    std::vector<module_location_pool_event> called;

    identifier id_0;
    identifier id_1;
    identifier id_2;
    identifier id_3;

    auto canceller =
        pool->observe_event([&called](module_location_pool_event const &event) { called.emplace_back(event); }).sync();

    XCTAssertEqual(called.size(), 1);
    XCTAssertEqual(called.at(0).type, module_location_pool_event_type::fetched);

    XCTAssertEqual(pool->elements().size(), 0);

    {
        pool->insert({.identifier = id_0});

        auto const &locations = pool->elements();
        XCTAssertEqual(locations.size(), 1);
        XCTAssertEqual(locations.at(0).value().identifier, id_0);

        XCTAssertEqual(called.size(), 2);
        XCTAssertEqual(called.at(1).type, module_location_pool_event_type::updated);
        XCTAssertEqual(called.at(1).inserted.size(), 1);
        XCTAssertEqual(called.at(1).inserted.at(0).first, 0);
        XCTAssertEqual(called.at(1).inserted.at(0).second.identifier, id_0);
        XCTAssertEqual(called.at(1).erased.size(), 0);
    }

    {
        pool->insert({.identifier = id_0});

        XCTAssertEqual(pool->elements().size(), 1);

        XCTAssertEqual(called.size(), 2);
    }

    {
        pool->insert({.identifier = id_1});

        auto const &locations = pool->elements();
        XCTAssertEqual(locations.size(), 2);
        XCTAssertEqual(locations.at(0).value().identifier, id_0);
        XCTAssertEqual(locations.at(1).value().identifier, id_1);

        XCTAssertEqual(called.size(), 3);
        XCTAssertEqual(called.at(2).type, module_location_pool_event_type::updated);
        XCTAssertEqual(called.at(2).inserted.size(), 1);
        XCTAssertEqual(called.at(2).inserted.at(0).first, 1);
        XCTAssertEqual(called.at(2).inserted.at(0).second.identifier, id_1);
        XCTAssertEqual(called.at(2).erased.size(), 0);
    }

    {
        pool->erase(id_0);
        auto const &locations = pool->elements();
        XCTAssertEqual(locations.size(), 2);
        XCTAssertFalse(locations.at(0).has_value());
        XCTAssertEqual(locations.at(1).value().identifier, id_1);

        XCTAssertEqual(called.size(), 4);
        XCTAssertEqual(called.at(3).type, module_location_pool_event_type::updated);
        XCTAssertEqual(called.at(3).inserted.size(), 0);
        XCTAssertEqual(called.at(3).erased.size(), 1);
    }

    {
        pool->insert({.identifier = id_2});

        auto const &locations = pool->elements();
        XCTAssertEqual(locations.size(), 2);
        XCTAssertEqual(locations.at(0).value().identifier, id_2);
        XCTAssertEqual(locations.at(1).value().identifier, id_1);

        XCTAssertEqual(called.size(), 5);
        XCTAssertEqual(called.at(4).type, module_location_pool_event_type::updated);
        XCTAssertEqual(called.at(4).inserted.size(), 1);
        XCTAssertEqual(called.at(4).inserted.at(0).first, 0);
        XCTAssertEqual(called.at(4).inserted.at(0).second.identifier, id_2);
        XCTAssertEqual(called.at(4).erased.size(), 0);
    }
}

@end

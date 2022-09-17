//
//  ae_module_location_pool_tests.mm
//

#import <XCTest/XCTest.h>
#include <audio_editor_core/ae_module_location.h>
#include <audio_editor_core/ae_module_location_pool.h>

using namespace yas;
using namespace yas::ae;

static time::range const dummy_range{0, 1};

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

    auto const id_0 = db::make_temporary_id();
    auto const id_1 = db::make_temporary_id();
    auto const id_2 = db::make_temporary_id();

    {
        pool->replace_all(
            {{id_0, dummy_range, 0, {}, 0}, {id_1, dummy_range, 0, {}, 0}, {id_2, dummy_range, 0, {}, 0}});

        auto const locations = pool->elements();
        XCTAssertEqual(locations.size(), 3);
        XCTAssertEqual(locations.at(0).value().identifier, id_0);
        XCTAssertEqual(locations.at(1).value().identifier, id_1);
        XCTAssertEqual(locations.at(2).value().identifier, id_2);

        XCTAssertEqual(called.size(), 2);
        XCTAssertEqual(called.at(1).type, module_location_pool_event_type::replaced);
    }

    auto const id_3 = db::make_temporary_id();
    auto const id_4 = db::make_temporary_id();

    {
        pool->replace_all({{id_3, dummy_range, 0, {}, 0}, {id_4, dummy_range, 0, {}, 0}});

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

    auto const id_0 = db::make_temporary_id();
    auto const id_1 = db::make_temporary_id();
    auto const id_2 = db::make_temporary_id();
    auto const id_3 = db::make_temporary_id();
    auto const id_4 = db::make_temporary_id();
    auto const id_5 = db::make_temporary_id();

    called.clear();

    {
        // 元が空なので全てinserted

        pool->update_all({{id_0, dummy_range, 0, {}, 0}, {id_2, dummy_range, 0, {}, 0}}, false);

        auto const locations = pool->elements();
        XCTAssertEqual(locations.size(), 2);
        XCTAssertEqual(locations.at(0).value().identifier, id_0);
        XCTAssertEqual(locations.at(1).value().identifier, id_2);

        XCTAssertEqual(called.size(), 1);
        XCTAssertEqual(called.at(0).type, module_location_pool_event_type::updated);
        XCTAssertEqual(called.at(0).inserted.size(), 2);
        XCTAssertEqual(called.at(0).inserted.at(0).first, 0);
        XCTAssertEqual(called.at(0).inserted.at(0).second.identifier, id_0);
        XCTAssertEqual(called.at(0).inserted.at(1).first, 1);
        XCTAssertEqual(called.at(0).inserted.at(1).second.identifier, id_2);
        XCTAssertEqual(called.at(0).erased.size(), 0);
        XCTAssertEqual(called.at(0).replaced.size(), 0);
    }

    called.clear();

    {
        // 追加された要素だけがinserted

        pool->update_all({{id_0, dummy_range, 0, {}, 0}, {id_1, dummy_range, 0, {}, 0}, {id_2, dummy_range, 0, {}, 0}},
                         false);

        auto const locations = pool->elements();
        XCTAssertEqual(locations.size(), 3);
        XCTAssertEqual(locations.at(0).value().identifier, id_0);
        XCTAssertEqual(locations.at(1).value().identifier, id_2);
        XCTAssertEqual(locations.at(2).value().identifier, id_1);

        XCTAssertEqual(called.size(), 1);
        XCTAssertEqual(called.at(0).type, module_location_pool_event_type::updated);
        XCTAssertEqual(called.at(0).inserted.size(), 1);
        XCTAssertEqual(called.at(0).inserted.at(0).first, 2);
        XCTAssertEqual(called.at(0).inserted.at(0).second.identifier, id_1);
        XCTAssertEqual(called.at(0).erased.size(), 0);
        XCTAssertEqual(called.at(0).replaced.size(), 0);
    }

    called.clear();

    {
        // 削除された要素がerased。減った要素のインデックスはnullで残る

        pool->update_all({{id_1, dummy_range, 0, {}, 0}, {id_2, dummy_range, 0, {}, 0}}, false);

        auto const locations = pool->elements();
        XCTAssertEqual(locations.size(), 3);
        XCTAssertFalse(locations.at(0).has_value());
        XCTAssertEqual(locations.at(1).value().identifier, id_2);
        XCTAssertEqual(locations.at(2).value().identifier, id_1);

        XCTAssertEqual(called.size(), 1);
        XCTAssertEqual(called.at(0).type, module_location_pool_event_type::updated);
        XCTAssertEqual(called.at(0).inserted.size(), 0);
        XCTAssertEqual(called.at(0).erased.size(), 1);
        XCTAssertEqual(called.at(0).erased.at(0).first, 0);
        XCTAssertEqual(called.at(0).erased.at(0).second.identifier, id_0);
        XCTAssertEqual(called.at(0).replaced.size(), 0);
    }

    called.clear();

    {
        // 追加したらnullの要素から埋められる

        pool->update_all({{id_1, dummy_range, 0, {}, 0},
                          {id_2, dummy_range, 0, {}, 0},
                          {id_3, dummy_range, 0, {}, 0},
                          {id_4, dummy_range, 0, {}, 0}},
                         false);
        auto const locations = pool->elements();
        XCTAssertEqual(locations.size(), 4);
        XCTAssertEqual(locations.at(0).value().identifier, id_3);
        XCTAssertEqual(locations.at(1).value().identifier, id_2);
        XCTAssertEqual(locations.at(2).value().identifier, id_1);
        XCTAssertEqual(locations.at(3).value().identifier, id_4);

        XCTAssertEqual(called.size(), 1);
        XCTAssertEqual(called.at(0).type, module_location_pool_event_type::updated);
        XCTAssertEqual(called.at(0).inserted.size(), 2);
        XCTAssertEqual(called.at(0).inserted.at(0).first, 0);
        XCTAssertEqual(called.at(0).inserted.at(0).second.identifier, id_3);
        XCTAssertEqual(called.at(0).inserted.at(1).first, 3);
        XCTAssertEqual(called.at(0).inserted.at(1).second.identifier, id_4);
        XCTAssertEqual(called.at(0).erased.size(), 0);
        XCTAssertEqual(called.at(0).replaced.size(), 0);
    }

    called.clear();

    {
        // inserted、erased、replaced全てのパターンが含まれる。要素が変わっていたらreplaced

        time::range const other_range{0, 2};

        pool->update_all({{id_1, dummy_range, 0, {}, 0}, {id_4, other_range, 0, {}, 0}, {id_5, dummy_range, 0, {}, 0}},
                         false);
        auto const locations = pool->elements();
        XCTAssertEqual(locations.size(), 4);
        XCTAssertEqual(locations.at(0).value().identifier, id_5);
        XCTAssertFalse(locations.at(1).has_value());
        XCTAssertEqual(locations.at(2).value().identifier, id_1);
        XCTAssertEqual(locations.at(3).value().identifier, id_4);

        XCTAssertEqual(called.size(), 1);
        XCTAssertEqual(called.at(0).type, module_location_pool_event_type::updated);
        XCTAssertEqual(called.at(0).inserted.size(), 1);
        XCTAssertEqual(called.at(0).inserted.at(0).first, 0);
        XCTAssertEqual(called.at(0).inserted.at(0).second.identifier, id_5);
        XCTAssertEqual(called.at(0).erased.size(), 2);
        XCTAssertEqual(called.at(0).erased.at(0).first, 0);
        XCTAssertEqual(called.at(0).erased.at(0).second.identifier, id_3);
        XCTAssertEqual(called.at(0).erased.at(1).first, 1);
        XCTAssertEqual(called.at(0).erased.at(1).second.identifier, id_2);
        XCTAssertEqual(called.at(0).replaced.size(), 1);
        XCTAssertEqual(called.at(0).replaced.at(0).first, 3);
        XCTAssertEqual(called.at(0).replaced.at(0).second.identifier, id_4);
        XCTAssertEqual(called.at(0).replaced.at(0).second.range, other_range);
    }

    called.clear();

    {
        // force_replacedをtrueにすると、変わっていない要素もreplacedに含まれる

        time::range const other_range{1, 3};

        pool->update_all({{id_0, dummy_range, 0, {}, 0}, {id_1, dummy_range, 0, {}, 0}, {id_5, other_range, 0, {}, 0}},
                         true);
        auto const locations = pool->elements();
        XCTAssertEqual(locations.size(), 4);
        XCTAssertEqual(locations.at(0).value().identifier, id_5);
        XCTAssertEqual(locations.at(1).value().identifier, id_0);
        XCTAssertEqual(locations.at(2).value().identifier, id_1);
        XCTAssertFalse(locations.at(3).has_value());

        XCTAssertEqual(called.size(), 1);
        XCTAssertEqual(called.at(0).type, module_location_pool_event_type::updated);
        XCTAssertEqual(called.at(0).inserted.size(), 1);
        XCTAssertEqual(called.at(0).inserted.at(0).first, 1);
        XCTAssertEqual(called.at(0).inserted.at(0).second.identifier, id_0);
        XCTAssertEqual(called.at(0).erased.size(), 1);
        XCTAssertEqual(called.at(0).erased.at(0).first, 3);
        XCTAssertEqual(called.at(0).erased.at(0).second.identifier, id_4);
        XCTAssertEqual(called.at(0).replaced.size(), 2);
        XCTAssertEqual(called.at(0).replaced.at(0).first, 2);
        XCTAssertEqual(called.at(0).replaced.at(0).second.identifier, id_1);
        XCTAssertEqual(called.at(0).replaced.at(1).first, 0);
        XCTAssertEqual(called.at(0).replaced.at(1).second.identifier, id_5);
    }

    canceller->cancel();
}

- (void)test_erase {
    auto const pool = module_location_pool::make_shared();
    std::vector<module_location_pool_event> called;

    auto const id_0 = db::make_temporary_id();
    auto const id_1 = db::make_temporary_id();
    auto const id_2 = db::make_temporary_id();
    auto const id_3 = db::make_temporary_id();

    pool->replace_all({{id_0, dummy_range, 0, {}, 0}, {id_1, dummy_range, 0, {}, 0}, {id_2, dummy_range, 0, {}, 0}});

    auto canceller =
        pool->observe_event([&called](module_location_pool_event const &event) { called.emplace_back(event); }).sync();

    XCTAssertEqual(called.size(), 1);
    XCTAssertEqual(called.at(0).type, module_location_pool_event_type::fetched);

    {
        pool->erase_for_id(id_1);

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
        XCTAssertEqual(called.at(1).replaced.size(), 0);
    }

    {
        pool->erase_for_id(id_3);

        XCTAssertEqual(called.size(), 2);
    }

    canceller->cancel();
}

- (void)test_insert {
    auto const pool = module_location_pool::make_shared();
    std::vector<module_location_pool_event> called;

    auto const id_0 = db::make_temporary_id();
    auto const id_1 = db::make_temporary_id();
    auto const id_2 = db::make_temporary_id();
    auto const id_3 = db::make_temporary_id();

    auto canceller =
        pool->observe_event([&called](module_location_pool_event const &event) { called.emplace_back(event); }).sync();

    XCTAssertEqual(called.size(), 1);
    XCTAssertEqual(called.at(0).type, module_location_pool_event_type::fetched);

    XCTAssertEqual(pool->elements().size(), 0);

    {
        pool->insert({id_0, dummy_range, 0, {}, 0});

        auto const &locations = pool->elements();
        XCTAssertEqual(locations.size(), 1);
        XCTAssertEqual(locations.at(0).value().identifier, id_0);

        XCTAssertEqual(called.size(), 2);
        XCTAssertEqual(called.at(1).type, module_location_pool_event_type::updated);
        XCTAssertEqual(called.at(1).inserted.size(), 1);
        XCTAssertEqual(called.at(1).inserted.at(0).first, 0);
        XCTAssertEqual(called.at(1).inserted.at(0).second.identifier, id_0);
        XCTAssertEqual(called.at(1).erased.size(), 0);
        XCTAssertEqual(called.at(1).replaced.size(), 0);
    }

    {
        pool->insert({id_0, dummy_range, 0, {}, 0});

        XCTAssertEqual(pool->elements().size(), 1);

        XCTAssertEqual(called.size(), 2);
    }

    {
        pool->insert({id_1, dummy_range, 0, {}, 0});

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
        XCTAssertEqual(called.at(2).replaced.size(), 0);
    }

    {
        pool->erase_for_id(id_0);
        auto const &locations = pool->elements();
        XCTAssertEqual(locations.size(), 2);
        XCTAssertFalse(locations.at(0).has_value());
        XCTAssertEqual(locations.at(1).value().identifier, id_1);

        XCTAssertEqual(called.size(), 4);
        XCTAssertEqual(called.at(3).type, module_location_pool_event_type::updated);
        XCTAssertEqual(called.at(3).inserted.size(), 0);
        XCTAssertEqual(called.at(3).erased.size(), 1);
        XCTAssertEqual(called.at(3).replaced.size(), 0);
    }

    {
        pool->insert({id_2, dummy_range, 0, {}, 0});

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
        XCTAssertEqual(called.at(4).replaced.size(), 0);
    }
}

- (void)test_replace {
    auto const pool = module_location_pool::make_shared();

    std::vector<module_location_pool_event> called;

    auto const id_0 = db::make_temporary_id();
    auto const id_1 = db::make_temporary_id();
    auto const id_2 = db::make_temporary_id();

    pool->replace_all({{id_0, dummy_range, 0, {}, 0}, {id_1, dummy_range, 1, {}, 0}, {id_2, dummy_range, 2, {}, 0}});

    auto canceller =
        pool->observe_event([&called](module_location_pool_event const &event) { called.emplace_back(event); }).sync();

    XCTAssertEqual(called.size(), 1);

    {
        pool->replace({id_1, dummy_range, 11, {}, 0});

        auto const &locations = pool->elements();
        XCTAssertEqual(locations.size(), 3);
        XCTAssertEqual(locations.at(0).value().sample_rate, 0);
        XCTAssertEqual(locations.at(1).value().sample_rate, 11);
        XCTAssertEqual(locations.at(2).value().sample_rate, 2);

        XCTAssertEqual(called.size(), 2);
        XCTAssertEqual(called.at(1).type, module_location_pool_event_type::updated);
        XCTAssertEqual(called.at(1).replaced.size(), 1);
        XCTAssertEqual(called.at(1).replaced.at(0).first, 1);
        XCTAssertEqual(called.at(1).replaced.at(0).second.identifier, id_1);
        XCTAssertEqual(called.at(1).inserted.size(), 0);
        XCTAssertEqual(called.at(1).erased.size(), 0);
    }

    {
        auto const id_other = db::make_temporary_id();

        pool->replace({id_other, dummy_range, 100, {}, 0});

        auto const &locations = pool->elements();
        XCTAssertEqual(locations.size(), 3);
        XCTAssertEqual(locations.at(0).value().sample_rate, 0);
        XCTAssertEqual(locations.at(1).value().sample_rate, 11);
        XCTAssertEqual(locations.at(2).value().sample_rate, 2);

        XCTAssertEqual(called.size(), 2);
    }
}

@end

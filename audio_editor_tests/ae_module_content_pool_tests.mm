//
//  ae_module_content_pool_tests.mm
//

#import <XCTest/XCTest.h>
#include <audio_editor_core/ae_module_content_pool.h>

using namespace yas;
using namespace yas::ae;

static time::range const dummy_range{0, 1};
static ui::size const dummy_scale = ui::size::zero();

namespace yas::ae::test_utils::module_content_pool {
static module_content make_module_content(db::object_id const &object_id, time::range const range = dummy_range,
                                          sample_rate_t const sample_rate = 0) {
    return {object_id, range, 0, false, sample_rate, {}, dummy_scale};
};
}

using namespace yas::ae::test_utils::module_content_pool;

@interface ae_module_content_pool_tests : XCTestCase

@end

@implementation ae_module_content_pool_tests

- (void)test_initial {
    auto const pool = module_content_pool::make_shared();

    XCTAssertEqual(pool->elements().size(), 0);
}

- (void)test_replace_all {
    auto const pool = module_content_pool::make_shared();

    std::vector<module_content_pool_event> called;

    auto canceller =
        pool->observe_event([&called](module_content_pool_event const &event) { called.emplace_back(event); }).sync();

    XCTAssertEqual(called.size(), 1);
    XCTAssertEqual(called.at(0).type, module_content_pool_event_type::fetched);

    auto const id_0 = db::make_temporary_id();
    auto const id_1 = db::make_temporary_id();
    auto const id_2 = db::make_temporary_id();

    {
        pool->replace_all({make_module_content(id_0), make_module_content(id_1), make_module_content(id_2)});

        auto const contents = pool->elements();
        XCTAssertEqual(contents.size(), 3);
        XCTAssertEqual(contents.at(0).value().identifier, id_0);
        XCTAssertEqual(contents.at(1).value().identifier, id_1);
        XCTAssertEqual(contents.at(2).value().identifier, id_2);

        XCTAssertEqual(called.size(), 2);
        XCTAssertEqual(called.at(1).type, module_content_pool_event_type::replaced);
    }

    auto const id_3 = db::make_temporary_id();
    auto const id_4 = db::make_temporary_id();

    {
        pool->replace_all({make_module_content(id_3), make_module_content(id_4)});

        auto const contents = pool->elements();
        XCTAssertEqual(contents.size(), 2);
        XCTAssertEqual(contents.at(0).value().identifier, id_3);
        XCTAssertEqual(contents.at(1).value().identifier, id_4);

        XCTAssertEqual(called.size(), 3);
        XCTAssertEqual(called.at(2).type, module_content_pool_event_type::replaced);
    }

    canceller->cancel();
}

- (void)test_update_all {
    auto const pool = module_content_pool::make_shared();
    std::vector<module_content_pool_event> called;

    auto canceller =
        pool->observe_event([&called](module_content_pool_event const &event) { called.emplace_back(event); }).sync();

    XCTAssertEqual(called.size(), 1);
    XCTAssertEqual(called.at(0).type, module_content_pool_event_type::fetched);

    auto const id_0 = db::make_temporary_id();
    auto const id_1 = db::make_temporary_id();
    auto const id_2 = db::make_temporary_id();
    auto const id_3 = db::make_temporary_id();
    auto const id_4 = db::make_temporary_id();
    auto const id_5 = db::make_temporary_id();

    called.clear();

    {
        // 元が空なので全てinserted

        pool->update_all({make_module_content(id_0), make_module_content(id_2)}, false);

        auto const contents = pool->elements();
        XCTAssertEqual(contents.size(), 2);
        XCTAssertEqual(contents.at(0).value().identifier, id_0);
        XCTAssertEqual(contents.at(1).value().identifier, id_2);

        XCTAssertEqual(called.size(), 1);
        XCTAssertEqual(called.at(0).type, module_content_pool_event_type::updated);
        XCTAssertEqual(called.at(0).inserted.size(), 2);
        XCTAssertTrue(called.at(0).inserted.contains(0));
        XCTAssertTrue(called.at(0).inserted.contains(1));
        XCTAssertEqual(called.at(0).erased.size(), 0);
        XCTAssertEqual(called.at(0).replaced.size(), 0);
    }

    called.clear();

    {
        // 追加された要素だけがinserted

        pool->update_all({make_module_content(id_0), make_module_content(id_1), make_module_content(id_2)}, false);

        auto const contents = pool->elements();
        XCTAssertEqual(contents.size(), 3);
        XCTAssertEqual(contents.at(0).value().identifier, id_0);
        XCTAssertEqual(contents.at(1).value().identifier, id_2);
        XCTAssertEqual(contents.at(2).value().identifier, id_1);

        XCTAssertEqual(called.size(), 1);
        XCTAssertEqual(called.at(0).type, module_content_pool_event_type::updated);
        XCTAssertEqual(called.at(0).inserted.size(), 1);
        XCTAssertTrue(called.at(0).inserted.contains(2));
        XCTAssertEqual(called.at(0).erased.size(), 0);
        XCTAssertEqual(called.at(0).replaced.size(), 0);
    }

    called.clear();

    {
        // 削除された要素がerased。減った要素のインデックスはnullで残る

        pool->update_all({make_module_content(id_1), make_module_content(id_2)}, false);

        auto const contents = pool->elements();
        XCTAssertEqual(contents.size(), 3);
        XCTAssertFalse(contents.at(0).has_value());
        XCTAssertEqual(contents.at(1).value().identifier, id_2);
        XCTAssertEqual(contents.at(2).value().identifier, id_1);

        XCTAssertEqual(called.size(), 1);
        XCTAssertEqual(called.at(0).type, module_content_pool_event_type::updated);
        XCTAssertEqual(called.at(0).inserted.size(), 0);
        XCTAssertEqual(called.at(0).erased.size(), 1);
        XCTAssertEqual(called.at(0).erased.at(0).identifier, id_0);
        XCTAssertEqual(called.at(0).replaced.size(), 0);
    }

    called.clear();

    {
        // 追加したらnullの要素から埋められる

        pool->update_all({make_module_content(id_1), make_module_content(id_2), make_module_content(id_3),
                          make_module_content(id_4)},
                         false);
        auto const contents = pool->elements();
        XCTAssertEqual(contents.size(), 4);
        XCTAssertEqual(contents.at(0).value().identifier, id_3);
        XCTAssertEqual(contents.at(1).value().identifier, id_2);
        XCTAssertEqual(contents.at(2).value().identifier, id_1);
        XCTAssertEqual(contents.at(3).value().identifier, id_4);

        XCTAssertEqual(called.size(), 1);
        XCTAssertEqual(called.at(0).type, module_content_pool_event_type::updated);
        XCTAssertEqual(called.at(0).inserted.size(), 2);
        XCTAssertTrue(called.at(0).inserted.contains(0));
        XCTAssertTrue(called.at(0).inserted.contains(3));
        XCTAssertEqual(called.at(0).erased.size(), 0);
        XCTAssertEqual(called.at(0).replaced.size(), 0);
    }

    called.clear();

    {
        // inserted、erased、replaced全てのパターンが含まれる。要素が変わっていたらreplaced

        time::range const other_range{0, 2};

        pool->update_all(
            {make_module_content(id_1), make_module_content(id_4, other_range, 0), make_module_content(id_5)}, false);
        auto const contents = pool->elements();
        XCTAssertEqual(contents.size(), 4);
        XCTAssertEqual(contents.at(0).value().identifier, id_5);
        XCTAssertFalse(contents.at(1).has_value());
        XCTAssertEqual(contents.at(2).value().identifier, id_1);
        XCTAssertEqual(contents.at(3).value().identifier, id_4);

        XCTAssertEqual(called.size(), 1);
        XCTAssertEqual(called.at(0).type, module_content_pool_event_type::updated);
        XCTAssertEqual(called.at(0).inserted.size(), 1);
        XCTAssertTrue(called.at(0).inserted.contains(0));
        XCTAssertEqual(called.at(0).erased.size(), 2);
        XCTAssertEqual(called.at(0).erased.at(0).identifier, id_3);
        XCTAssertEqual(called.at(0).erased.at(1).identifier, id_2);
        XCTAssertEqual(called.at(0).replaced.size(), 1);
        XCTAssertTrue(called.at(0).replaced.contains(3));
    }

    called.clear();

    {
        // force_replacedをtrueにすると、変わっていない要素もreplacedに含まれる

        time::range const other_range{1, 3};

        pool->update_all({make_module_content(id_0), make_module_content(id_1), make_module_content(id_5)}, true);
        auto const contents = pool->elements();
        XCTAssertEqual(contents.size(), 4);
        XCTAssertEqual(contents.at(0).value().identifier, id_5);
        XCTAssertEqual(contents.at(1).value().identifier, id_0);
        XCTAssertEqual(contents.at(2).value().identifier, id_1);
        XCTAssertFalse(contents.at(3).has_value());

        XCTAssertEqual(called.size(), 1);
        XCTAssertEqual(called.at(0).type, module_content_pool_event_type::updated);
        XCTAssertEqual(called.at(0).inserted.size(), 1);
        XCTAssertTrue(called.at(0).inserted.contains(1));
        XCTAssertEqual(called.at(0).erased.size(), 1);
        XCTAssertEqual(called.at(0).erased.at(3).identifier, id_4);
        XCTAssertEqual(called.at(0).replaced.size(), 2);
        XCTAssertTrue(called.at(0).replaced.contains(2));
        XCTAssertTrue(called.at(0).replaced.contains(0));
    }

    canceller->cancel();
}

- (void)test_erase {
    auto const pool = module_content_pool::make_shared();
    std::vector<module_content_pool_event> called;

    auto const id_0 = db::make_temporary_id();
    auto const id_1 = db::make_temporary_id();
    auto const id_2 = db::make_temporary_id();
    auto const id_3 = db::make_temporary_id();

    pool->replace_all({make_module_content(id_0), make_module_content(id_1), make_module_content(id_2)});

    auto canceller =
        pool->observe_event([&called](module_content_pool_event const &event) { called.emplace_back(event); }).sync();

    XCTAssertEqual(called.size(), 1);
    XCTAssertEqual(called.at(0).type, module_content_pool_event_type::fetched);

    {
        pool->erase_for_id(id_1);

        auto const &contents = pool->elements();
        XCTAssertEqual(contents.size(), 3);
        XCTAssertEqual(contents.at(0).value().identifier, id_0);
        XCTAssertFalse(contents.at(1).has_value());
        XCTAssertEqual(contents.at(2).value().identifier, id_2);
        XCTAssertEqual(called.size(), 2);
        XCTAssertEqual(called.at(1).type, module_content_pool_event_type::updated);
        XCTAssertEqual(called.at(1).inserted.size(), 0);
        XCTAssertEqual(called.at(1).erased.size(), 1);
        XCTAssertEqual(called.at(1).erased.at(1).identifier, id_1);
        XCTAssertEqual(called.at(1).replaced.size(), 0);
    }

    {
        pool->erase_for_id(id_3);

        XCTAssertEqual(called.size(), 2);
    }

    canceller->cancel();
}

- (void)test_insert {
    auto const pool = module_content_pool::make_shared();
    std::vector<module_content_pool_event> called;

    auto const id_0 = db::make_temporary_id();
    auto const id_1 = db::make_temporary_id();
    auto const id_2 = db::make_temporary_id();
    auto const id_3 = db::make_temporary_id();

    auto canceller =
        pool->observe_event([&called](module_content_pool_event const &event) { called.emplace_back(event); }).sync();

    XCTAssertEqual(called.size(), 1);
    XCTAssertEqual(called.at(0).type, module_content_pool_event_type::fetched);

    XCTAssertEqual(pool->elements().size(), 0);

    {
        pool->insert(make_module_content(id_0));

        auto const &contents = pool->elements();
        XCTAssertEqual(contents.size(), 1);
        XCTAssertEqual(contents.at(0).value().identifier, id_0);

        XCTAssertEqual(called.size(), 2);
        XCTAssertEqual(called.at(1).type, module_content_pool_event_type::updated);
        XCTAssertEqual(called.at(1).inserted.size(), 1);
        XCTAssertTrue(called.at(1).inserted.contains(0));
        XCTAssertEqual(called.at(1).erased.size(), 0);
        XCTAssertEqual(called.at(1).replaced.size(), 0);
    }

    {
        pool->insert(make_module_content(id_0));

        XCTAssertEqual(pool->elements().size(), 1);

        XCTAssertEqual(called.size(), 2);
    }

    {
        pool->insert(make_module_content(id_1));

        auto const &contents = pool->elements();
        XCTAssertEqual(contents.size(), 2);
        XCTAssertEqual(contents.at(0).value().identifier, id_0);
        XCTAssertEqual(contents.at(1).value().identifier, id_1);

        XCTAssertEqual(called.size(), 3);
        XCTAssertEqual(called.at(2).type, module_content_pool_event_type::updated);
        XCTAssertEqual(called.at(2).inserted.size(), 1);
        XCTAssertTrue(called.at(2).inserted.contains(1));
        XCTAssertEqual(called.at(2).erased.size(), 0);
        XCTAssertEqual(called.at(2).replaced.size(), 0);
    }

    {
        pool->erase_for_id(id_0);
        auto const &contents = pool->elements();
        XCTAssertEqual(contents.size(), 2);
        XCTAssertFalse(contents.at(0).has_value());
        XCTAssertEqual(contents.at(1).value().identifier, id_1);

        XCTAssertEqual(called.size(), 4);
        XCTAssertEqual(called.at(3).type, module_content_pool_event_type::updated);
        XCTAssertEqual(called.at(3).inserted.size(), 0);
        XCTAssertEqual(called.at(3).erased.size(), 1);
        XCTAssertEqual(called.at(3).replaced.size(), 0);
    }

    {
        pool->insert(make_module_content(id_2));

        auto const &contents = pool->elements();
        XCTAssertEqual(contents.size(), 2);
        XCTAssertEqual(contents.at(0).value().identifier, id_2);
        XCTAssertEqual(contents.at(1).value().identifier, id_1);

        XCTAssertEqual(called.size(), 5);
        XCTAssertEqual(called.at(4).type, module_content_pool_event_type::updated);
        XCTAssertEqual(called.at(4).inserted.size(), 1);
        XCTAssertTrue(called.at(4).inserted.contains(0));
        XCTAssertEqual(called.at(4).erased.size(), 0);
        XCTAssertEqual(called.at(4).replaced.size(), 0);
    }
}

- (void)test_replace {
    auto const pool = module_content_pool::make_shared();

    std::vector<module_content_pool_event> called;

    auto const id_0 = db::make_temporary_id();
    auto const id_1 = db::make_temporary_id();
    auto const id_2 = db::make_temporary_id();

    pool->replace_all({make_module_content(id_0, dummy_range, 0), make_module_content(id_1, dummy_range, 1),
                       make_module_content(id_2, dummy_range, 2)});

    auto canceller =
        pool->observe_event([&called](module_content_pool_event const &event) { called.emplace_back(event); }).sync();

    XCTAssertEqual(called.size(), 1);

    {
        pool->replace(make_module_content(id_1, dummy_range, 11));

        auto const &contents = pool->elements();
        XCTAssertEqual(contents.size(), 3);
        XCTAssertEqual(contents.at(0).value().sample_rate, 0);
        XCTAssertEqual(contents.at(1).value().sample_rate, 11);
        XCTAssertEqual(contents.at(2).value().sample_rate, 2);

        XCTAssertEqual(called.size(), 2);
        XCTAssertEqual(called.at(1).type, module_content_pool_event_type::updated);
        XCTAssertEqual(called.at(1).replaced.size(), 1);
        XCTAssertTrue(called.at(1).replaced.contains(1));
        XCTAssertEqual(called.at(1).inserted.size(), 0);
        XCTAssertEqual(called.at(1).erased.size(), 0);
    }

    {
        auto const id_other = db::make_temporary_id();

        pool->replace(make_module_content(id_other, dummy_range, 100));

        auto const &contents = pool->elements();
        XCTAssertEqual(contents.size(), 3);
        XCTAssertEqual(contents.at(0).value().sample_rate, 0);
        XCTAssertEqual(contents.at(1).value().sample_rate, 11);
        XCTAssertEqual(contents.at(2).value().sample_rate, 2);

        XCTAssertEqual(called.size(), 2);
    }
}

@end

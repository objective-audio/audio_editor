//
//  ae_marker_pool_tests.mm
//

#import <XCTest/XCTest.h>
#import <audio_editor_core/ae_db_constants.h>
#import <audio_editor_core/ae_db_marker.h>
#import <audio_editor_core/ae_db_utils.h>
#import <audio_editor_core/ae_marker_pool.h>

using namespace yas;
using namespace yas::ae;

namespace yas::ae::marker_pool_test_utils {
enum db_called {
    add,
    remove,
    update,
};

struct db_add_called {
    frame_index_t frame;
    std::string name;
};

struct db_remove_called {
    frame_index_t frame;
};

struct db_update_called {
    frame_index_t frame;
    marker_object marker;
};

struct database_mock final : database_for_marker_pool {
    std::function<void(frame_index_t const, std::string const &)> add_handler = [](auto const, auto const &) {};
    std::function<void(frame_index_t const)> remove_handler = [](auto const) {};
    std::function<void(frame_index_t const, marker_object const &)> update_handler = [](auto const, auto const &) {};

    db_marker add_marker(frame_index_t const frame, std::string const &name) override {
        this->add_handler(frame, name);

        auto const model = database_utils::make_model();
        auto const object = db::object::make_shared(model.entity(db_constants::marker_name::entity));
        object->set_attribute_value(db_constants::marker_name::attribute::frame, db::value{frame});
        object->set_attribute_value(db_constants::marker_name::attribute::name, db::value{name});
        return {object};
    }

    void remove_marker(frame_index_t const &frame) override {
        this->remove_handler(frame);
    }

    void update_marker(frame_index_t const &prev_frame, marker_object const &marker) override {
        this->update_handler(prev_frame, marker);
    }
};
}

using namespace marker_pool_test_utils;

@interface ae_marker_pool_tests : XCTestCase

@end

@implementation ae_marker_pool_tests

- (void)test_initial {
    auto const database = std::make_shared<database_mock>();
    auto const pool = marker_pool::make_shared(database.get());

    XCTAssertEqual(pool->markers().size(), 0);
}

- (void)test_revert {
    auto const database = std::make_shared<database_mock>();
    auto const pool = marker_pool::make_shared(database.get());

    auto const id_0 = db::make_temporary_id();
    auto const id_1 = db::make_temporary_id();
    auto const id_m1 = db::make_temporary_id();

    pool->revert_markers({{id_0, {0, ""}}, {id_1, {1, ""}}, {id_m1, {-1, ""}}});

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
    auto const database = std::make_shared<database_mock>();
    auto const pool = marker_pool::make_shared(database.get());

    std::vector<db_called> called;
    std::vector<db_add_called> add_called;

    database->add_handler = [&called, &add_called](frame_index_t const frame, std::string const &name) {
        called.emplace_back(db_called::add);
        add_called.emplace_back(db_add_called{.frame = frame, .name = name});
    };
    database->remove_handler = [&called](auto const) { called.emplace_back(db_called::remove); };
    database->update_handler = [&called](auto const, auto const &) { called.emplace_back(db_called::update); };

    pool->insert_marker(0);

    XCTAssertEqual(pool->markers().size(), 1);
    XCTAssertEqual(pool->markers().at(0).value.frame, 0);

    XCTAssertEqual(called.size(), 1);
    XCTAssertEqual(called.at(0), db_called::add);
    XCTAssertEqual(add_called.size(), 1);
    XCTAssertEqual(add_called.at(0).frame, 0);

    pool->insert_marker(1);

    XCTAssertEqual(pool->markers().size(), 2);
    XCTAssertEqual(pool->markers().at(1).value.frame, 1);

    XCTAssertEqual(called.size(), 2);
    XCTAssertEqual(called.at(1), db_called::add);
    XCTAssertEqual(add_called.size(), 2);
    XCTAssertEqual(add_called.at(1).frame, 1);
}

- (void)test_erase_at {
    auto const database = std::make_shared<database_mock>();
    auto const pool = marker_pool::make_shared(database.get());

    auto const id_0 = db::make_temporary_id();
    auto const id_1 = db::make_temporary_id();
    auto const id_m1 = db::make_temporary_id();

    pool->revert_markers({{id_0, {0, ""}}, {id_1, {1, ""}}, {id_m1, {-1, ""}}});

    std::vector<db_called> called;
    std::vector<db_remove_called> remove_called;

    database->add_handler = [&called](auto const, auto const &) { called.emplace_back(db_called::add); };
    database->remove_handler = [&called, &remove_called](auto const frame) {
        remove_called.emplace_back(db_remove_called{.frame = frame});
        called.emplace_back(db_called::remove);
    };
    database->update_handler = [&called](auto const, auto const &) { called.emplace_back(db_called::update); };

    pool->erase_at(0);

    XCTAssertEqual(pool->markers().size(), 2);
    XCTAssertEqual(pool->markers().at(-1).identifier, id_m1);
    XCTAssertEqual(pool->markers().at(1).identifier, id_1);

    XCTAssertEqual(called.size(), 1);
    XCTAssertEqual(called.at(0), db_called::remove);
    XCTAssertEqual(remove_called.size(), 1);
    XCTAssertEqual(remove_called.at(0).frame, 0);
}

- (void)test_erase_range {
    auto const database = std::make_shared<database_mock>();
    auto const pool = marker_pool::make_shared(database.get());

    auto const id_0 = db::make_temporary_id();
    auto const id_1 = db::make_temporary_id();
    auto const id_2 = db::make_temporary_id();
    auto const id_3 = db::make_temporary_id();

    pool->revert_markers({{id_0, {0, ""}}, {id_1, {1, ""}}, {id_2, {2, ""}}, {id_3, {3, ""}}});

    std::vector<db_called> called;
    std::vector<db_remove_called> remove_called;

    database->add_handler = [&called](auto const, auto const &) { called.emplace_back(db_called::add); };
    database->remove_handler = [&called, &remove_called](auto const frame) {
        remove_called.emplace_back(db_remove_called{.frame = frame});
        called.emplace_back(db_called::remove);
    };
    database->update_handler = [&called](auto const, auto const &) { called.emplace_back(db_called::update); };

    pool->erase_range({1, 2});

    XCTAssertEqual(pool->markers().size(), 2);
    XCTAssertEqual(pool->markers().at(0).identifier, id_0);
    XCTAssertEqual(pool->markers().at(3).identifier, id_3);

    XCTAssertEqual(called.size(), 2);
    XCTAssertEqual(called.at(0), db_called::remove);
    XCTAssertEqual(called.at(1), db_called::remove);
    XCTAssertEqual(remove_called.size(), 2);
    XCTAssertEqual(remove_called.at(0).frame, 1);
    XCTAssertEqual(remove_called.at(1).frame, 2);
}

- (void)test_move_marker_at {
    auto const database = std::make_shared<database_mock>();
    auto const pool = marker_pool::make_shared(database.get());

    auto const id_0 = db::make_temporary_id();
    auto const id_1 = db::make_temporary_id();
    auto const id_m1 = db::make_temporary_id();

    pool->revert_markers({{id_m1, {-1, ""}}, {id_0, {0, ""}}, {id_1, {1, ""}}});

    std::vector<db_called> called;
    std::vector<db_update_called> update_called;

    database->add_handler = [&called](auto const, auto const &) { called.emplace_back(db_called::add); };
    database->remove_handler = [&called](auto const frame) { called.emplace_back(db_called::remove); };
    database->update_handler = [&called, &update_called](auto const frame, auto const &marker) {
        update_called.emplace_back(db_update_called{.frame = frame, .marker = marker});
        called.emplace_back(db_called::update);
    };

    pool->move_at(0, 2);

    XCTAssertEqual(pool->markers().size(), 3);
    XCTAssertEqual(pool->markers().at(-1).identifier, id_m1);
    XCTAssertEqual(pool->markers().at(1).identifier, id_1);
    // moveしてもidentifierは変わらない
    XCTAssertEqual(pool->markers().at(2).identifier, id_0);
    XCTAssertEqual(pool->markers().at(2).value.frame, 2);

    XCTAssertEqual(called.size(), 1);
    XCTAssertEqual(called.at(0), db_called::update);
    XCTAssertEqual(update_called.size(), 1);
    XCTAssertEqual(update_called.at(0).frame, 0);
    XCTAssertEqual(update_called.at(0).marker.value.frame, 2);
    XCTAssertEqual(update_called.at(0).marker.identifier, id_0);
}

- (void)test_move_offset_from {
    auto const database = std::make_shared<database_mock>();
    auto const pool = marker_pool::make_shared(database.get());

    auto const id_0 = db::make_temporary_id();
    auto const id_1 = db::make_temporary_id();
    auto const id_m1 = db::make_temporary_id();

    pool->revert_markers({{id_m1, {-1, ""}}, {id_0, {0, ""}}, {id_1, {1, ""}}});

    std::vector<db_called> called;
    std::vector<db_update_called> update_called;

    database->add_handler = [&called](auto const, auto const &) { called.emplace_back(db_called::add); };
    database->remove_handler = [&called](auto const frame) { called.emplace_back(db_called::remove); };
    database->update_handler = [&called, &update_called](auto const frame, auto const &marker) {
        update_called.emplace_back(db_update_called{.frame = frame, .marker = marker});
        called.emplace_back(db_called::update);
    };

    pool->move_offset_from(0, 2);

    XCTAssertEqual(pool->markers().size(), 3);
    XCTAssertEqual(pool->markers().at(-1).identifier, id_m1);
    // moveしてもidentifierは変わらない
    XCTAssertEqual(pool->markers().at(2).identifier, id_0);
    XCTAssertEqual(pool->markers().at(2).value.frame, 2);
    XCTAssertEqual(pool->markers().at(3).identifier, id_1);
    XCTAssertEqual(pool->markers().at(3).value.frame, 3);

    XCTAssertEqual(called.size(), 2);
    XCTAssertEqual(called.at(0), db_called::update);
    XCTAssertEqual(called.at(1), db_called::update);
    XCTAssertEqual(update_called.size(), 2);
    XCTAssertEqual(update_called.at(0).frame, 0);
    XCTAssertEqual(update_called.at(0).marker.value.frame, 2);
    XCTAssertEqual(update_called.at(0).marker.identifier, id_0);
    XCTAssertEqual(update_called.at(1).frame, 1);
    XCTAssertEqual(update_called.at(1).marker.value.frame, 3);
    XCTAssertEqual(update_called.at(1).marker.identifier, id_1);
}

- (void)test_observe {
    auto const database = std::make_shared<database_mock>();
    auto const pool = marker_pool::make_shared(database.get());

    auto const id_0 = db::make_temporary_id();
    auto const id_1 = db::make_temporary_id();

    pool->revert_markers({{id_0, {0, "0"}}, {id_1, {1, "1"}}});

    std::vector<marker_pool_event> called;

    auto canceller = pool->observe_event([&called](auto const &event) { called.emplace_back(event); }).sync();

    XCTAssertEqual(called.size(), 1);
    XCTAssertEqual(called.at(0).type, marker_pool_event_type::any);
    XCTAssertEqual(called.at(0).inserted, std::nullopt);
    XCTAssertEqual(called.at(0).erased, std::nullopt);

    pool->insert_marker(-1, "-1");

    XCTAssertEqual(called.size(), 2);
    XCTAssertEqual(called.at(1).type, marker_pool_event_type::inserted);
    XCTAssertEqual(called.at(1).inserted.value().value.frame, -1);

    // nameだけを変更
    pool->update_marker(1, {id_1, {1, "1b"}});

    XCTAssertEqual(called.size(), 3);
    XCTAssertEqual(called.at(2).type, marker_pool_event_type::replaced);
    XCTAssertEqual(called.at(2).erased.value().identifier, id_1);
    XCTAssertEqual(called.at(2).erased.value().value.name, "1");
    XCTAssertEqual(called.at(2).inserted.value().identifier, id_1);
    XCTAssertEqual(called.at(2).inserted.value().value.name, "1b");

    // frameだけを変更
    pool->update_marker(1, {id_1, {10, "1b"}});

    XCTAssertEqual(called.size(), 5);
    XCTAssertEqual(called.at(3).type, marker_pool_event_type::erased);
    XCTAssertEqual(called.at(3).erased.value().identifier, id_1);
    XCTAssertEqual(called.at(3).erased.value().value.name, "1b");
    XCTAssertEqual(called.at(4).type, marker_pool_event_type::inserted);
    XCTAssertEqual(called.at(4).inserted.value().identifier, id_1);
    XCTAssertEqual(called.at(4).inserted.value().value.name, "1b");

    pool->erase_at(0);

    XCTAssertEqual(called.size(), 6);
    XCTAssertEqual(called.at(5).type, marker_pool_event_type::erased);
    XCTAssertEqual(called.at(5).erased.value().identifier, id_0);

    canceller->cancel();
}

@end

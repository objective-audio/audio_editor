//
//  ae_marker_pool_tests.mm
//

#import <XCTest/XCTest.h>
#import <ae-core/project_editing/features/ae_db_utils.h>
#import <ae-core/project_editing/features/ae_marker_pool.h>
#import <ae-core/project_editing/value_types/ae_db_constants.h>
#import <ae-core/project_editing/value_types/ae_db_marker.h>

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
    object_id object_id;
};

struct db_update_called {
    object_id object_id;
    marker_object marker;
};

struct database_mock final : database_for_marker_pool {
    std::function<void(frame_index_t const, std::string const &)> add_handler = [](auto const, auto const &) {};
    std::function<void(object_id const)> remove_handler = [](auto const) {};
    std::function<void(object_id const, marker_object const &)> update_handler = [](auto const, auto const &) {};

    db_marker add_marker(frame_index_t const frame, std::string const &name) override {
        this->add_handler(frame, name);

        auto const model = database_utils::make_model();
        auto const object = db::object::make_shared(model.entity(db_constants::marker_name::entity));
        object->set_attribute_value(db_constants::marker_name::attribute::frame, db::value{frame});
        object->set_attribute_value(db_constants::marker_name::attribute::name, db::value{name});
        return {object};
    }

    void remove_marker(object_id const &object_id) override {
        this->remove_handler(object_id);
    }

    void update_marker(object_id const &object_id, marker_object const &marker) override {
        this->update_handler(object_id, marker);
    }
};
}  // namespace yas::ae::marker_pool_test_utils

using namespace marker_pool_test_utils;

@interface ae_marker_pool_tests : XCTestCase

@end

@implementation ae_marker_pool_tests

- (void)test_initial {
    auto const database = std::make_shared<database_mock>();
    auto const pool = std::make_shared<ae::marker_pool>(database.get());

    XCTAssertEqual(pool->markers().size(), 0);
}

- (void)test_revert {
    auto const database = std::make_shared<database_mock>();
    auto const pool = std::make_shared<ae::marker_pool>(database.get());

    auto const id_0 = db::make_temporary_id();
    auto const id_1 = db::make_temporary_id();
    auto const id_m1 = db::make_temporary_id();

    pool->revert({{id_0, {0, ""}}, {id_1, {1, ""}}, {id_m1, {-1, ""}}});

    XCTAssertEqual(pool->markers().size(), 3);

    auto iterator = pool->markers().begin();
    XCTAssertEqual(iterator->first.frame, -1);
    XCTAssertEqual(iterator->first.object_id, id_m1);
    XCTAssertEqual(iterator->second.value.frame, -1);
    XCTAssertEqual(iterator->second.identifier, id_m1);
    ++iterator;
    XCTAssertEqual(iterator->first.frame, 0);
    XCTAssertEqual(iterator->first.object_id, id_0);
    XCTAssertEqual(iterator->second.value.frame, 0);
    XCTAssertEqual(iterator->second.identifier, id_0);
    ++iterator;
    XCTAssertEqual(iterator->first.frame, 1);
    XCTAssertEqual(iterator->first.object_id, id_1);
    XCTAssertEqual(iterator->second.value.frame, 1);
    XCTAssertEqual(iterator->second.identifier, id_1);
}

- (void)test_insert {
    auto const database = std::make_shared<database_mock>();
    auto const pool = std::make_shared<ae::marker_pool>(database.get());

    std::vector<db_called> called;
    std::vector<db_add_called> add_called;

    database->add_handler = [&called, &add_called](frame_index_t const frame, std::string const &name) {
        called.emplace_back(db_called::add);
        add_called.emplace_back(db_add_called{.frame = frame, .name = name});
    };
    database->remove_handler = [&called](auto const) { called.emplace_back(db_called::remove); };
    database->update_handler = [&called](auto const, auto const &) { called.emplace_back(db_called::update); };

    auto const index0 = pool->insert(0);
    XCTAssertTrue(index0.has_value());

    XCTAssertEqual(pool->markers().size(), 1);
    XCTAssertEqual(pool->markers().at(index0.value()).value.frame, 0);

    XCTAssertEqual(called.size(), 1);
    XCTAssertEqual(called.at(0), db_called::add);
    XCTAssertEqual(add_called.size(), 1);
    XCTAssertEqual(add_called.at(0).frame, 0);

    auto const index1 = pool->insert(1);
    XCTAssertTrue(index1.has_value());

    XCTAssertEqual(pool->markers().size(), 2);
    XCTAssertEqual(pool->markers().at(index1.value()).value.frame, 1);

    XCTAssertEqual(called.size(), 2);
    XCTAssertEqual(called.at(1), db_called::add);
    XCTAssertEqual(add_called.size(), 2);
    XCTAssertEqual(add_called.at(1).frame, 1);
}

- (void)test_erase_at {
    auto const database = std::make_shared<database_mock>();
    auto const pool = std::make_shared<ae::marker_pool>(database.get());

    auto const id_0 = db::make_temporary_id();
    auto const id_1 = db::make_temporary_id();
    auto const id_m1 = db::make_temporary_id();
    marker_index const index_1{id_1, 1};
    marker_index const index_m1{id_m1, -1};

    pool->revert({{id_0, {0, ""}}, {id_1, {1, ""}}, {id_m1, {-1, ""}}});

    std::vector<db_called> called;
    std::vector<db_remove_called> remove_called;

    database->add_handler = [&called](auto const, auto const &) { called.emplace_back(db_called::add); };
    database->remove_handler = [&called, &remove_called](auto const object_id) {
        remove_called.emplace_back(db_remove_called{.object_id = object_id});
        called.emplace_back(db_called::remove);
    };
    database->update_handler = [&called](auto const, auto const &) { called.emplace_back(db_called::update); };

    pool->erase(0);

    XCTAssertEqual(pool->markers().size(), 2);
    XCTAssertEqual(pool->markers().at(index_m1).identifier, id_m1);
    XCTAssertEqual(pool->markers().at(index_1).identifier, id_1);

    XCTAssertEqual(called.size(), 1);
    XCTAssertEqual(called.at(0), db_called::remove);
    XCTAssertEqual(remove_called.size(), 1);
    XCTAssertEqual(remove_called.at(0).object_id, id_0);
}

- (void)test_erase_range {
    auto const database = std::make_shared<database_mock>();
    auto const pool = std::make_shared<ae::marker_pool>(database.get());

    auto const id_0 = db::make_temporary_id();
    auto const id_1 = db::make_temporary_id();
    auto const id_2 = db::make_temporary_id();
    auto const id_3 = db::make_temporary_id();
    marker_index const index_0{id_0, 0};
    marker_index const index_3{id_3, 3};

    pool->revert({{id_0, {0, ""}}, {id_1, {1, ""}}, {id_2, {2, ""}}, {id_3, {3, ""}}});

    std::vector<db_called> called;
    std::vector<db_remove_called> remove_called;

    database->add_handler = [&called](auto const, auto const &) { called.emplace_back(db_called::add); };
    database->remove_handler = [&called, &remove_called](auto const object_id) {
        remove_called.emplace_back(db_remove_called{.object_id = object_id});
        called.emplace_back(db_called::remove);
    };
    database->update_handler = [&called](auto const, auto const &) { called.emplace_back(db_called::update); };

    pool->erase({1, 2});

    XCTAssertEqual(pool->markers().size(), 2);
    XCTAssertEqual(pool->markers().at(index_0).identifier, id_0);
    XCTAssertEqual(pool->markers().at(index_3).identifier, id_3);

    XCTAssertEqual(called.size(), 2);
    XCTAssertEqual(called.at(0), db_called::remove);
    XCTAssertEqual(called.at(1), db_called::remove);
    XCTAssertEqual(remove_called.size(), 2);
    XCTAssertEqual(remove_called.at(0).object_id, id_1);
    XCTAssertEqual(remove_called.at(1).object_id, id_2);
}

- (void)test_move_marker_at {
    auto const database = std::make_shared<database_mock>();
    auto const pool = std::make_shared<ae::marker_pool>(database.get());

    auto const id_0 = db::make_temporary_id();
    auto const id_1 = db::make_temporary_id();
    auto const id_m1 = db::make_temporary_id();
    marker_index const index_0{id_0, 0};
    marker_index const index_1{id_1, 1};
    marker_index const index_m1{id_m1, -1};
    marker_index const index_2{id_0, 2};

    pool->revert({{id_m1, {-1, ""}}, {id_0, {0, ""}}, {id_1, {1, ""}}});

    std::vector<db_called> called;
    std::vector<db_update_called> update_called;

    database->add_handler = [&called](auto const, auto const &) { called.emplace_back(db_called::add); };
    database->remove_handler = [&called](auto const frame) { called.emplace_back(db_called::remove); };
    database->update_handler = [&called, &update_called](auto const object_id, auto const &marker) {
        update_called.emplace_back(db_update_called{.object_id = object_id, .marker = marker});
        called.emplace_back(db_called::update);
    };

    pool->move(index_0, 2);

    XCTAssertEqual(pool->markers().size(), 3);
    XCTAssertEqual(pool->markers().at(index_m1).identifier, id_m1);
    XCTAssertEqual(pool->markers().at(index_1).identifier, id_1);
    // moveしてもidentifierは変わらない
    XCTAssertEqual(pool->markers().at(index_2).identifier, id_0);
    XCTAssertEqual(pool->markers().at(index_2).value.frame, 2);

    XCTAssertEqual(called.size(), 1);
    XCTAssertEqual(called.at(0), db_called::update);
    XCTAssertEqual(update_called.size(), 1);
    XCTAssertEqual(update_called.at(0).object_id, id_0);
    XCTAssertEqual(update_called.at(0).marker.value.frame, 2);
    XCTAssertEqual(update_called.at(0).marker.identifier, id_0);
}

- (void)test_move_offset_from {
    auto const database = std::make_shared<database_mock>();
    auto const pool = std::make_shared<ae::marker_pool>(database.get());

    auto const id_0 = db::make_temporary_id();
    auto const id_1 = db::make_temporary_id();
    auto const id_m1 = db::make_temporary_id();
    marker_index const index_m1{id_m1, -1};
    marker_index const index_2{id_0, 2};
    marker_index const index_3{id_1, 3};

    pool->revert({{id_m1, {-1, ""}}, {id_0, {0, ""}}, {id_1, {1, ""}}});

    std::vector<db_called> called;
    std::vector<db_update_called> update_called;

    database->add_handler = [&called](auto const, auto const &) { called.emplace_back(db_called::add); };
    database->remove_handler = [&called](auto const frame) { called.emplace_back(db_called::remove); };
    database->update_handler = [&called, &update_called](auto const object_id, auto const &marker) {
        update_called.emplace_back(db_update_called{.object_id = object_id, .marker = marker});
        called.emplace_back(db_called::update);
    };

    pool->move_offset_from(0, 2);

    XCTAssertEqual(pool->markers().size(), 3);
    XCTAssertEqual(pool->markers().at(index_m1).identifier, id_m1);
    // moveしてもidentifierは変わらない
    XCTAssertEqual(pool->markers().at(index_2).identifier, id_0);
    XCTAssertEqual(pool->markers().at(index_2).value.frame, 2);
    XCTAssertEqual(pool->markers().at(index_3).identifier, id_1);
    XCTAssertEqual(pool->markers().at(index_3).value.frame, 3);

    XCTAssertEqual(called.size(), 2);
    XCTAssertEqual(called.at(0), db_called::update);
    XCTAssertEqual(called.at(1), db_called::update);
    XCTAssertEqual(update_called.size(), 2);
    XCTAssertEqual(update_called.at(0).object_id, id_0);
    XCTAssertEqual(update_called.at(0).marker.value.frame, 2);
    XCTAssertEqual(update_called.at(0).marker.identifier, id_0);
    XCTAssertEqual(update_called.at(1).object_id, id_1);
    XCTAssertEqual(update_called.at(1).marker.value.frame, 3);
    XCTAssertEqual(update_called.at(1).marker.identifier, id_1);
}

- (void)test_observe {
    auto const database = std::make_shared<database_mock>();
    auto const pool = std::make_shared<ae::marker_pool>(database.get());

    auto const id_0 = db::make_temporary_id();
    auto const id_1 = db::make_temporary_id();
    marker_index const index_1{id_1, 1};

    pool->revert({{id_0, {0, "0"}}, {id_1, {1, "1"}}});

    std::vector<marker_pool_event> called;

    auto canceller = pool->observe_event([&called](auto const &event) { called.emplace_back(event); }).sync();

    XCTAssertEqual(called.size(), 1);
    XCTAssertEqual(called.at(0).type, marker_pool_event_type::any);
    XCTAssertEqual(called.at(0).inserted, std::nullopt);
    XCTAssertEqual(called.at(0).erased, std::nullopt);

    pool->insert(-1, "-1");

    XCTAssertEqual(called.size(), 2);
    XCTAssertEqual(called.at(1).type, marker_pool_event_type::inserted);
    XCTAssertEqual(called.at(1).inserted.value().value.frame, -1);

    // nameだけを変更
    pool->update(index_1, {id_1, {1, "1b"}});

    XCTAssertEqual(called.size(), 3);
    XCTAssertEqual(called.at(2).type, marker_pool_event_type::replaced);
    XCTAssertEqual(called.at(2).erased.value().identifier, id_1);
    XCTAssertEqual(called.at(2).erased.value().value.name, "1");
    XCTAssertEqual(called.at(2).inserted.value().identifier, id_1);
    XCTAssertEqual(called.at(2).inserted.value().value.name, "1b");

    // frameだけを変更
    pool->update(index_1, {id_1, {10, "1b"}});

    XCTAssertEqual(called.size(), 5);
    XCTAssertEqual(called.at(3).type, marker_pool_event_type::erased);
    XCTAssertEqual(called.at(3).erased.value().identifier, id_1);
    XCTAssertEqual(called.at(3).erased.value().value.name, "1b");
    XCTAssertEqual(called.at(4).type, marker_pool_event_type::inserted);
    XCTAssertEqual(called.at(4).inserted.value().identifier, id_1);
    XCTAssertEqual(called.at(4).inserted.value().value.name, "1b");

    pool->erase(0);

    XCTAssertEqual(called.size(), 6);
    XCTAssertEqual(called.at(5).type, marker_pool_event_type::erased);
    XCTAssertEqual(called.at(5).erased.value().identifier, id_0);

    canceller->cancel();
}

@end

//
//  module_pool_tests.mm
//

#import <XCTest/XCTest.h>
#import <ae-core/project_editing/features/db_utils.h>
#import <ae-core/project_editing/features/module_pool.h>
#import <ae-core/project_editing/value_types/db_constants.h>
#import <ae-core/project_editing/value_types/db_module.h>

using namespace yas;
using namespace yas::ae;

namespace yas::ae::module_pool_test_utils {
struct database_mock : database_for_module_pool {
    db_module add_module(module const &params) override {
        auto const model = database_utils::make_model();
        auto const object = db::object::make_shared(model.entity(db_constants::module_name::entity));
        object->set_attribute_value(db_constants::module_name::attribute::name, db::value{params.name});
        object->set_attribute_value(db_constants::module_name::attribute::range_frame, db::value{params.range.frame});
        object->set_attribute_value(db_constants::module_name::attribute::range_length, db::value{params.range.length});
        object->set_attribute_value(db_constants::module_name::attribute::file_frame, db::value{params.file_frame});
        object->set_attribute_value(db_constants::module_name::attribute::file_name, db::value{params.file_name});
        object->set_attribute_value(db_constants::module_name::attribute::track, db::value{params.track});
        return db_module{object};
    }

    void remove_module(object_id const &) override {
    }

    void update_module(object_id const &, module const &) override {
    }
};
}  // namespace yas::ae::module_pool_test_utils

using namespace yas::ae::module_pool_test_utils;

@interface module_pool_tests : XCTestCase

@end

@implementation module_pool_tests

- (void)test_insert_module_and_notify {
    auto const database = std::make_shared<database_mock>();
    auto const module_pool = std::make_shared<ae::module_pool>(database.get());

    XCTAssertEqual(module_pool->modules().size(), 0);

    module const module_params_1{"", {0, 4}, 0, 0, ""};

    auto const module1_index = module_pool->insert_module_and_notify(module_params_1);

    XCTAssertEqual(module_pool->modules().size(), 1);
    XCTAssertEqual(module_pool->module_at(module1_index.value()).value().index(), module1_index.value());

    module const module_params_2{"", {4, 3}, 0, 4, ""};

    auto const module2_index = module_pool->insert_module_and_notify(module_params_2);

    XCTAssertEqual(module_pool->modules().size(), 2);
    XCTAssertEqual(module_pool->module_at(module2_index.value()).value().index(), module2_index.value());

    module const module_params_3{"", {-2, 2}, 0, 7, ""};

    auto const module3_index = module_pool->insert_module_and_notify(module_params_3);

    XCTAssertEqual(module_pool->modules().size(), 3);
    XCTAssertEqual(module_pool->module_at(module3_index.value()).value().index(), module3_index.value());

    module const module_params_4{"", {0, 4}, 1, 0, ""};

    auto const module4_index = module_pool->insert_module_and_notify(module_params_4);

    XCTAssertEqual(module_pool->modules().size(), 4);
    XCTAssertEqual(module_pool->module_at(module4_index.value()).value().index(), module4_index.value());

    auto iterator = module_pool->modules().begin();
    XCTAssertEqual(iterator->first, module3_index);
    ++iterator;
    XCTAssertEqual(iterator->first, module1_index);
    ++iterator;
    XCTAssertEqual(iterator->first, module4_index);
    ++iterator;
    XCTAssertEqual(iterator->first, module2_index);
}

- (void)test_erase_module_and_notify {
    auto const database = std::make_shared<database_mock>();
    auto const module_pool = std::make_shared<ae::module_pool>(database.get());

    module_object const module1{db::make_temporary_id(), {"module_1", {0, 4}, 0, 0, ""}};
    module_object const module2{db::make_temporary_id(), {"module_2", {5, 3}, 0, 5, ""}};
    module_object const module3{db::make_temporary_id(), {"module_3", {0, 4}, 1, 0, ""}};

    module_pool->revert_modules_and_notify({module1, module2, module3});

    XCTAssertEqual(module_pool->modules().size(), 3);

    module_pool->erase_module_and_notify(module1.index());

    XCTAssertEqual(module_pool->modules().size(), 2);
    XCTAssertFalse(module_pool->modules().contains(module1.index()));
    XCTAssertTrue(module_pool->modules().contains(module2.index()));
    XCTAssertTrue(module_pool->modules().contains(module3.index()));
}

- (void)test_observe_event {
    struct called_event {
        module_pool_event_type type;
        std::optional<module_object> module{std::nullopt};
        module_pool_module_map_t modules;
    };

    std::vector<called_event> called;

    auto const database = std::make_shared<database_mock>();
    auto const module_pool = std::make_shared<ae::module_pool>(database.get());

    module_object const module1{db::make_temporary_id(), {"module_1", {0, 4}, 0, 0, ""}};
    module_object const module2{db::make_temporary_id(), {"module_2", {7, 5}, 0, 7, ""}};

    module_pool->revert_modules_and_notify({module1, module2});

    auto canceller =
        module_pool
            ->observe_event([&called](auto const &event) {
                called.emplace_back(called_event{.type = event.type, .module = event.module, .modules = event.modules});
            })
            .sync();

    XCTAssertEqual(called.size(), 1);
    XCTAssertEqual(called.at(0).type, module_pool_event_type::any);
    XCTAssertEqual(called.at(0).modules.size(), 2);
    XCTAssertEqual(called.at(0).module, std::nullopt);

    auto const module3_index = module_pool->insert_module_and_notify({"module_3", {4, 3}, 0, 4, ""});

    XCTAssertEqual(called.size(), 2);
    XCTAssertEqual(called.at(1).type, module_pool_event_type::inserted);
    XCTAssertEqual(called.at(1).modules.size(), 3);
    XCTAssertEqual(called.at(1).module.value().index(), module3_index);

    module_pool->erase_module_and_notify(module1.index());

    XCTAssertEqual(called.size(), 3);
    XCTAssertEqual(called.at(2).type, module_pool_event_type::erased);
    XCTAssertEqual(called.at(2).modules.size(), 2);
    XCTAssertEqual(called.at(2).module.value().identifier, module1.identifier);

    module_pool->revert_modules_and_notify({module2});

    XCTAssertEqual(called.size(), 4);
    XCTAssertEqual(called.at(3).type, module_pool_event_type::reverted);
    XCTAssertEqual(called.at(3).modules.size(), 1);
    XCTAssertEqual(called.at(3).module, std::nullopt);

    canceller->cancel();
}

- (void)test_modules_at_tracks_and_frame {
    auto const database = std::make_shared<database_mock>();
    auto const module_pool = std::make_shared<ae::module_pool>(database.get());

    module_object const module_t0_0_1{db::make_temporary_id(), {"module_1", {0, 1}, 0, 0, ""}};
    module_object const module_t0_1_2{db::make_temporary_id(), {"module_2", {1, 2}, 0, 0, ""}};
    module_object const module_t1_0_2{db::make_temporary_id(), {"module_1", {0, 2}, 1, 0, ""}};
    module_object const module_t1_2_2{db::make_temporary_id(), {"module_1", {2, 2}, 1, 0, ""}};
    module_object const module_t2_1_2{db::make_temporary_id(), {"module_1", {1, 2}, 2, 0, ""}};

    module_pool->revert_modules_and_notify({module_t0_0_1, module_t0_1_2, module_t1_0_2, module_t2_1_2, module_t1_2_2});

    {
        auto const modules = module_pool->modules_at({}, -1);
        XCTAssertEqual(modules.size(), 0);
    }

    {
        auto const modules = module_pool->modules_at({}, 0);
        XCTAssertEqual(modules.size(), 2);
        XCTAssertTrue(modules.contains(module_t0_0_1.index()));
        XCTAssertTrue(modules.contains(module_t1_0_2.index()));
    }

    {
        auto const modules = module_pool->modules_at({}, 1);
        XCTAssertEqual(modules.size(), 3);
        XCTAssertTrue(modules.contains(module_t0_1_2.index()));
        XCTAssertTrue(modules.contains(module_t1_0_2.index()));
        XCTAssertTrue(modules.contains(module_t2_1_2.index()));
    }

    {
        auto const modules = module_pool->modules_at({-1}, 1);
        XCTAssertEqual(modules.size(), 0);
    }

    {
        auto const modules = module_pool->modules_at({0}, 1);
        XCTAssertEqual(modules.size(), 1);
        XCTAssertTrue(modules.contains(module_t0_1_2.index()));
    }

    {
        auto const modules = module_pool->modules_at({1}, 1);
        XCTAssertEqual(modules.size(), 1);
        XCTAssertTrue(modules.contains(module_t1_0_2.index()));
    }

    {
        auto const modules = module_pool->modules_at({0, 1}, 1);
        XCTAssertEqual(modules.size(), 2);
        XCTAssertTrue(modules.contains(module_t0_1_2.index()));
        XCTAssertTrue(modules.contains(module_t1_0_2.index()));
    }

    {
        auto const modules = module_pool->modules_at({2}, 1);
        XCTAssertEqual(modules.size(), 1);
        XCTAssertTrue(modules.contains(module_t2_1_2.index()));
    }

    {
        auto const modules = module_pool->modules_at({3}, 1);
        XCTAssertEqual(modules.size(), 0);
    }

    {
        auto const modules = module_pool->modules_at({}, 2);
        XCTAssertEqual(modules.size(), 3);
        XCTAssertTrue(modules.contains(module_t0_1_2.index()));
        XCTAssertTrue(modules.contains(module_t1_2_2.index()));
        XCTAssertTrue(modules.contains(module_t2_1_2.index()));
    }

    {
        auto const modules = module_pool->modules_at({}, 3);
        XCTAssertEqual(modules.size(), 1);
        XCTAssertTrue(modules.contains(module_t1_2_2.index()));
    }

    {
        auto const modules = module_pool->modules_at({}, 4);
        XCTAssertEqual(modules.size(), 0);
    }
}

- (void)test_module_at_index {
    auto const database = std::make_shared<database_mock>();
    auto const module_pool = std::make_shared<ae::module_pool>(database.get());

    module_object const module1{db::make_temporary_id(), {"module_1", {0, 1}, 0, 0, ""}};
    module_object const module2{db::make_temporary_id(), {"module_2", {1, 2}, 1, 1, ""}};
    module_object const other_module{db::make_temporary_id(), {"other_module", {100, 100}, 0, 100, ""}};

    module_pool->revert_modules_and_notify({module1, module2});

    XCTAssertEqual(module_pool->module_at(other_module.index()), std::nullopt);

    auto const stored_module1 = module_pool->module_at(module1.index());
    XCTAssertEqual(stored_module1.value().value.name, "module_1");
    auto const stored_module2 = module_pool->module_at(module2.index());
    XCTAssertEqual(stored_module2.value().value.name, "module_2");
}

- (void)test_previous_jumpable_frame_at {
    auto const database = std::make_shared<database_mock>();
    auto const module_pool = std::make_shared<ae::module_pool>(database.get());

    module_object const module1{db::make_temporary_id(), {"module_1", {0, 1}, 0, 0, ""}};
    module_object const module2{db::make_temporary_id(), {"module_2", {1, 2}, 1, 1, ""}};
    module_object const module3{db::make_temporary_id(), {"module_3", {4, 3}, 2, 4, ""}};

    module_pool->revert_modules_and_notify({module1, module2, module3});

    XCTAssertEqual(module_pool->previous_jumpable_frame(-1), std::nullopt);
    XCTAssertEqual(module_pool->previous_jumpable_frame(0), std::nullopt);
    XCTAssertEqual(module_pool->previous_jumpable_frame(1), 0);
    XCTAssertEqual(module_pool->previous_jumpable_frame(2), 1);
    XCTAssertEqual(module_pool->previous_jumpable_frame(3), 1);
    XCTAssertEqual(module_pool->previous_jumpable_frame(4), 3);
    XCTAssertEqual(module_pool->previous_jumpable_frame(5), 4);
    XCTAssertEqual(module_pool->previous_jumpable_frame(6), 4);
    XCTAssertEqual(module_pool->previous_jumpable_frame(7), 4);
    XCTAssertEqual(module_pool->previous_jumpable_frame(8), 7);
}

- (void)test_next_jumpable_frame_at {
    auto const database = std::make_shared<database_mock>();
    auto const module_pool = std::make_shared<ae::module_pool>(database.get());

    module_object const module1{db::make_temporary_id(), {"module_1", {0, 1}, 0, 0, ""}};
    module_object const module2{db::make_temporary_id(), {"module_2", {1, 2}, 1, 1, ""}};
    module_object const module3{db::make_temporary_id(), {"module_3", {4, 3}, 2, 4, ""}};

    module_pool->revert_modules_and_notify({module1, module2, module3});

    XCTAssertEqual(module_pool->next_jumpable_frame(-1), 0);
    XCTAssertEqual(module_pool->next_jumpable_frame(0), 1);
    XCTAssertEqual(module_pool->next_jumpable_frame(1), 3);
    XCTAssertEqual(module_pool->next_jumpable_frame(2), 3);
    XCTAssertEqual(module_pool->next_jumpable_frame(3), 4);
    XCTAssertEqual(module_pool->next_jumpable_frame(4), 7);
    XCTAssertEqual(module_pool->next_jumpable_frame(5), 7);
    XCTAssertEqual(module_pool->next_jumpable_frame(6), 7);
    XCTAssertFalse(module_pool->next_jumpable_frame(7).has_value());
}

- (void)test_splittable_modules_at {
    auto const database = std::make_shared<database_mock>();
    auto const module_pool = std::make_shared<ae::module_pool>(database.get());

    module_object const module_t0_0_2{db::make_temporary_id(), {"", {0, 3}, 0, 0, ""}};
    module_object const module_t1_1_2{db::make_temporary_id(), {"", {1, 3}, 1, 0, ""}};
    module_object const module_t2_2_2{db::make_temporary_id(), {"", {2, 3}, 2, 0, ""}};

    module_pool->revert_modules_and_notify({module_t0_0_2, module_t1_1_2, module_t2_2_2});

    {
        auto const modules = module_pool->splittable_modules_at({}, -1);
        XCTAssertEqual(modules.size(), 0);
    }

    {
        auto const modules = module_pool->splittable_modules_at({}, 0);
        XCTAssertEqual(modules.size(), 0);
    }

    {
        auto const modules = module_pool->splittable_modules_at({}, 1);
        XCTAssertEqual(modules.size(), 1);
        XCTAssertTrue(modules.contains(module_t0_0_2.index()));
    }

    {
        auto const modules = module_pool->splittable_modules_at({}, 2);
        XCTAssertEqual(modules.size(), 2);
        XCTAssertTrue(modules.contains(module_t0_0_2.index()));
        XCTAssertTrue(modules.contains(module_t1_1_2.index()));
    }

    {
        auto const modules = module_pool->splittable_modules_at({}, 3);
        XCTAssertEqual(modules.size(), 2);
        XCTAssertTrue(modules.contains(module_t1_1_2.index()));
        XCTAssertTrue(modules.contains(module_t2_2_2.index()));
    }

    {
        auto const modules = module_pool->splittable_modules_at({}, 4);
        XCTAssertEqual(modules.size(), 1);
        XCTAssertTrue(modules.contains(module_t2_2_2.index()));
    }

    {
        auto const modules = module_pool->splittable_modules_at({}, 5);
        XCTAssertEqual(modules.size(), 0);
    }
}

- (void)test_split_by_all_tracks {
    auto const database = std::make_shared<database_mock>();
    auto const module_pool = std::make_shared<ae::module_pool>(database.get());

    module_object const src_module_a{db::make_temporary_id(), {"split_module_name_a", {0, 8}, 0, 0, ""}};
    module_object const src_module_b{db::make_temporary_id(), {"split_module_name_b", {2, 3}, 1, 0, ""}};

    module_pool->revert_modules_and_notify({src_module_a, src_module_b});

    module_pool->split_at({}, -1);
    module_pool->split_at({}, 0);
    module_pool->split_at({}, 8);
    module_pool->split_at({}, 9);

    XCTAssertEqual(module_pool->modules().size(), 2);

    {
        module_pool->split_at({}, 1);

        XCTAssertEqual(module_pool->modules().size(), 3);

        auto iterator = module_pool->modules().begin();

        XCTAssertEqual(iterator->first.range, (time::range{0, 1}));
        XCTAssertEqual(iterator->second.value.name, "split_module_name_a");
        XCTAssertEqual(iterator->second.value.range, (time::range{0, 1}));
        XCTAssertEqual(iterator->second.value.file_frame, 0);
        XCTAssertEqual(iterator->second.value.track, 0);

        ++iterator;

        XCTAssertEqual(iterator->first.range, (time::range{1, 7}));
        XCTAssertEqual(iterator->second.value.name, "split_module_name_a");
        XCTAssertEqual(iterator->second.value.range, (time::range{1, 7}));
        XCTAssertEqual(iterator->second.value.file_frame, 1);
        XCTAssertEqual(iterator->second.value.track, 0);

        ++iterator;

        XCTAssertEqual(iterator->first.range, (time::range{2, 3}));
        XCTAssertEqual(iterator->second.value.name, "split_module_name_b");
        XCTAssertEqual(iterator->second.value.range, (time::range{2, 3}));
        XCTAssertEqual(iterator->second.value.file_frame, 0);
        XCTAssertEqual(iterator->second.value.track, 1);
    }

    {
        module_pool->split_at({}, 3);

        XCTAssertEqual(module_pool->modules().size(), 5);

        auto iterator = module_pool->modules().begin();

        XCTAssertEqual(iterator->first.range, (time::range{0, 1}));
        XCTAssertEqual(iterator->second.value.name, "split_module_name_a");
        XCTAssertEqual(iterator->second.value.range, (time::range{0, 1}));
        XCTAssertEqual(iterator->second.value.file_frame, 0);
        XCTAssertEqual(iterator->second.value.track, 0);

        ++iterator;

        XCTAssertEqual(iterator->first.range, (time::range{1, 2}));
        XCTAssertEqual(iterator->second.value.name, "split_module_name_a");
        XCTAssertEqual(iterator->second.value.range, (time::range{1, 2}));
        XCTAssertEqual(iterator->second.value.file_frame, 1);
        XCTAssertEqual(iterator->second.value.track, 0);

        ++iterator;

        XCTAssertEqual(iterator->first.range, (time::range{2, 1}));
        XCTAssertEqual(iterator->second.value.name, "split_module_name_b");
        XCTAssertEqual(iterator->second.value.range, (time::range{2, 1}));
        XCTAssertEqual(iterator->second.value.file_frame, 0);
        XCTAssertEqual(iterator->second.value.track, 1);

        ++iterator;

        XCTAssertEqual(iterator->first.range, (time::range{3, 2}));
        XCTAssertEqual(iterator->second.value.name, "split_module_name_b");
        XCTAssertEqual(iterator->second.value.range, (time::range{3, 2}));
        XCTAssertEqual(iterator->second.value.file_frame, 1);
        XCTAssertEqual(iterator->second.value.track, 1);

        ++iterator;

        XCTAssertEqual(iterator->first.range, (time::range{3, 5}));
        XCTAssertEqual(iterator->second.value.name, "split_module_name_a");
        XCTAssertEqual(iterator->second.value.range, (time::range{3, 5}));
        XCTAssertEqual(iterator->second.value.file_frame, 3);
        XCTAssertEqual(iterator->second.value.track, 0);
    }
}

- (void)test_split_by_single_track {
    auto const database = std::make_shared<database_mock>();
    auto const module_pool = std::make_shared<ae::module_pool>(database.get());

    module_object const src_module_a{db::make_temporary_id(), {"split_module_name_a", {0, 2}, 0, 0, ""}};
    module_object const src_module_b{db::make_temporary_id(), {"split_module_name_b", {0, 3}, 1, 0, ""}};

    module_pool->revert_modules_and_notify({src_module_a, src_module_b});

    XCTAssertEqual(module_pool->modules().size(), 2);

    module_pool->split_at({1}, 1);

    XCTAssertEqual(module_pool->modules().size(), 3);

    auto iterator = module_pool->modules().begin();

    XCTAssertEqual(iterator->first.range, (time::range{0, 1}));
    XCTAssertEqual(iterator->first.track, 1);

    ++iterator;

    XCTAssertEqual(iterator->first.range, (time::range{0, 2}));
    XCTAssertEqual(iterator->first.track, 0);

    ++iterator;

    XCTAssertEqual(iterator->first.range, (time::range{1, 2}));
    XCTAssertEqual(iterator->first.track, 1);
}

- (void)test_erase_by_all_tracks {
    auto const database = std::make_shared<database_mock>();
    auto const module_pool = std::make_shared<ae::module_pool>(database.get());

    module_pool->revert_modules_and_notify({{db::make_temporary_id(), {"module_1", {0, 2}, 0, 0, ""}},
                                            {db::make_temporary_id(), {"module_2", {2, 2}, 0, 2, ""}},
                                            {db::make_temporary_id(), {"module_3", {4, 2}, 0, 4, ""}},
                                            {db::make_temporary_id(), {"module_4", {3, 1}, 1, 0, ""}}});

    XCTAssertEqual(module_pool->modules().size(), 4);

    module_pool->erase_at({}, 3);

    XCTAssertEqual(module_pool->modules().size(), 2);

    auto iterator = module_pool->modules().begin();

    XCTAssertEqual(iterator->first.range, (time::range{0, 2}));
    XCTAssertEqual(iterator->second.value.name, "module_1");
    XCTAssertEqual(iterator->second.value.range, (time::range{0, 2}));
    XCTAssertEqual(iterator->second.value.file_frame, 0);

    ++iterator;

    XCTAssertEqual(iterator->first.range, (time::range{4, 2}));
    XCTAssertEqual(iterator->second.value.name, "module_3");
    XCTAssertEqual(iterator->second.value.range, (time::range{4, 2}));
    XCTAssertEqual(iterator->second.value.file_frame, 4);
}

- (void)test_erase_by_single_track {
    auto const database = std::make_shared<database_mock>();
    auto const module_pool = std::make_shared<ae::module_pool>(database.get());

    module_pool->revert_modules_and_notify({{db::make_temporary_id(), {"module_1", {0, 1}, 0, 0, ""}},
                                            {db::make_temporary_id(), {"module_2", {1, 1}, 0, 0, ""}},
                                            {db::make_temporary_id(), {"module_3", {0, 1}, 1, 0, ""}}});

    XCTAssertEqual(module_pool->modules().size(), 3);

    module_pool->erase_at({0}, 0);

    XCTAssertEqual(module_pool->modules().size(), 2);

    auto iterator = module_pool->modules().begin();

    XCTAssertEqual(iterator->first.range, (time::range{0, 1}));
    XCTAssertEqual(iterator->second.value.name, "module_3");

    ++iterator;

    XCTAssertEqual(iterator->first.range, (time::range{1, 1}));
    XCTAssertEqual(iterator->second.value.name, "module_2");
}

- (void)test_drop_head_by_all_tracks {
    auto const database = std::make_shared<database_mock>();
    auto const module_pool = std::make_shared<ae::module_pool>(database.get());

    module_object const src_module_a{db::make_temporary_id(), {"drop_head_module_a", {10, 4}, 0, 100, ""}};
    module_object const src_module_b{db::make_temporary_id(), {"drop_head_module_b", {11, 3}, 1, 200, ""}};

    module_pool->revert_modules_and_notify({src_module_a, src_module_b});

    module_pool->drop_head_at({}, 9);
    module_pool->drop_head_at({}, 10);
    module_pool->drop_head_at({}, 14);
    module_pool->drop_head_at({}, 15);

    XCTAssertEqual(module_pool->modules().size(), 2);

    {
        auto iterator = module_pool->modules().begin();
        XCTAssertEqual(iterator->first.range, (time::range{10, 4}));
        ++iterator;
        XCTAssertEqual(iterator->first.range, (time::range{11, 3}));
    }

    module_pool->drop_head_at({}, 11);

    XCTAssertEqual(module_pool->modules().size(), 2);

    {
        auto iterator = module_pool->modules().begin();

        XCTAssertEqual(iterator->first.range, (time::range{11, 3}));
        XCTAssertEqual(iterator->second.value.name, "drop_head_module_a");
        XCTAssertEqual(iterator->second.value.range, (time::range{11, 3}));
        XCTAssertEqual(iterator->second.value.file_frame, 101);
        XCTAssertEqual(iterator->second.value.track, 0);

        ++iterator;

        XCTAssertEqual(iterator->first.range, (time::range{11, 3}));
        XCTAssertEqual(iterator->second.value.name, "drop_head_module_b");
        XCTAssertEqual(iterator->second.value.range, (time::range{11, 3}));
        XCTAssertEqual(iterator->second.value.file_frame, 200);
        XCTAssertEqual(iterator->second.value.track, 1);
    }

    module_pool->drop_head_at({}, 12);

    XCTAssertEqual(module_pool->modules().size(), 2);

    {
        auto iterator = module_pool->modules().begin();

        XCTAssertEqual(iterator->first.range, (time::range{12, 2}));
        XCTAssertEqual(iterator->second.value.name, "drop_head_module_a");
        XCTAssertEqual(iterator->second.value.range, (time::range{12, 2}));
        XCTAssertEqual(iterator->second.value.file_frame, 102);
        XCTAssertEqual(iterator->second.value.track, 0);

        ++iterator;

        XCTAssertEqual(iterator->first.range, (time::range{12, 2}));
        XCTAssertEqual(iterator->second.value.name, "drop_head_module_b");
        XCTAssertEqual(iterator->second.value.range, (time::range{12, 2}));
        XCTAssertEqual(iterator->second.value.file_frame, 201);
        XCTAssertEqual(iterator->second.value.track, 1);
    }
}

- (void)test_drop_head_by_single_track {
    auto const database = std::make_shared<database_mock>();
    auto const module_pool = std::make_shared<ae::module_pool>(database.get());

    module_object const src_module_a{db::make_temporary_id(), {"drop_head_module_a", {0, 2}, 0, 100, ""}};
    module_object const src_module_b{db::make_temporary_id(), {"drop_head_module_b", {0, 3}, 1, 200, ""}};

    module_pool->revert_modules_and_notify({src_module_a, src_module_b});

    module_pool->drop_head_at({0}, 1);

    XCTAssertEqual(module_pool->modules().size(), 2);

    {
        auto iterator = module_pool->modules().begin();

        XCTAssertEqual(iterator->first.range, (time::range{0, 3}));
        XCTAssertEqual(iterator->second.value.name, "drop_head_module_b");
        XCTAssertEqual(iterator->second.value.range, (time::range{0, 3}));
        XCTAssertEqual(iterator->second.value.file_frame, 200);
        XCTAssertEqual(iterator->second.value.track, 1);

        ++iterator;

        XCTAssertEqual(iterator->first.range, (time::range{1, 1}));
        XCTAssertEqual(iterator->second.value.name, "drop_head_module_a");
        XCTAssertEqual(iterator->second.value.range, (time::range{1, 1}));
        XCTAssertEqual(iterator->second.value.file_frame, 101);
        XCTAssertEqual(iterator->second.value.track, 0);
    }
}

- (void)test_drop_tail_by_all_tracks {
    auto const database = std::make_shared<database_mock>();
    auto const module_pool = std::make_shared<ae::module_pool>(database.get());

    module_object const src_module_a{db::make_temporary_id(), {"drop_tail_module_a", {10, 3}, 0, 100, ""}};
    module_object const src_module_b{db::make_temporary_id(), {"drop_tail_module_b", {10, 4}, 1, 200, ""}};

    module_pool->revert_modules_and_notify({src_module_a, src_module_b});

    module_pool->drop_tail_at({}, 9);
    module_pool->drop_tail_at({}, 10);
    module_pool->drop_tail_at({}, 14);
    module_pool->drop_tail_at({}, 15);

    XCTAssertEqual(module_pool->modules().size(), 2);

    {
        auto iterator = module_pool->modules().begin();
        XCTAssertEqual(iterator->first.range, (time::range{10, 3}));
        XCTAssertEqual(iterator->second.value.track, 0);
        ++iterator;
        XCTAssertEqual(iterator->first.range, (time::range{10, 4}));
        XCTAssertEqual(iterator->second.value.track, 1);
    }

    module_pool->drop_tail_at({}, 13);

    XCTAssertEqual(module_pool->modules().size(), 2);

    {
        auto iterator = module_pool->modules().begin();

        XCTAssertEqual(iterator->first.range, (time::range{10, 3}));
        XCTAssertEqual(iterator->second.value.name, "drop_tail_module_a");
        XCTAssertEqual(iterator->second.value.range, (time::range{10, 3}));
        XCTAssertEqual(iterator->second.value.file_frame, 100);
        XCTAssertEqual(iterator->second.value.track, 0);

        ++iterator;

        XCTAssertEqual(iterator->first.range, (time::range{10, 3}));
        XCTAssertEqual(iterator->second.value.name, "drop_tail_module_b");
        XCTAssertEqual(iterator->second.value.range, (time::range{10, 3}));
        XCTAssertEqual(iterator->second.value.file_frame, 200);
        XCTAssertEqual(iterator->second.value.track, 1);
    }

    module_pool->drop_tail_at({}, 12);

    XCTAssertEqual(module_pool->modules().size(), 2);

    {
        auto iterator = module_pool->modules().begin();

        XCTAssertEqual(iterator->first.range, (time::range{10, 2}));
        XCTAssertEqual(iterator->second.value.name, "drop_tail_module_a");
        XCTAssertEqual(iterator->second.value.range, (time::range{10, 2}));
        XCTAssertEqual(iterator->second.value.file_frame, 100);
        XCTAssertEqual(iterator->second.value.track, 0);

        ++iterator;

        XCTAssertEqual(iterator->first.range, (time::range{10, 2}));
        XCTAssertEqual(iterator->second.value.name, "drop_tail_module_b");
        XCTAssertEqual(iterator->second.value.range, (time::range{10, 2}));
        XCTAssertEqual(iterator->second.value.file_frame, 200);
        XCTAssertEqual(iterator->second.value.track, 1);
    }
}

- (void)test_drop_tail_by_single_track {
    auto const database = std::make_shared<database_mock>();
    auto const module_pool = std::make_shared<ae::module_pool>(database.get());

    module_object const src_module_a{db::make_temporary_id(), {"drop_tail_module_a", {0, 3}, 0, 100, ""}};
    module_object const src_module_b{db::make_temporary_id(), {"drop_tail_module_b", {1, 2}, 1, 200, ""}};

    module_pool->revert_modules_and_notify({src_module_a, src_module_b});

    module_pool->drop_tail_at({0}, 2);

    XCTAssertEqual(module_pool->modules().size(), 2);

    {
        auto iterator = module_pool->modules().begin();

        XCTAssertEqual(iterator->first.range, (time::range{0, 2}));
        XCTAssertEqual(iterator->second.value.name, "drop_tail_module_a");
        XCTAssertEqual(iterator->second.value.range, (time::range{0, 2}));
        XCTAssertEqual(iterator->second.value.file_frame, 100);
        XCTAssertEqual(iterator->second.value.track, 0);

        ++iterator;

        XCTAssertEqual(iterator->first.range, (time::range{1, 2}));
        XCTAssertEqual(iterator->second.value.name, "drop_tail_module_b");
        XCTAssertEqual(iterator->second.value.range, (time::range{1, 2}));
        XCTAssertEqual(iterator->second.value.file_frame, 200);
        XCTAssertEqual(iterator->second.value.track, 1);
    }
}

- (void)test_overwrite_module_middle_cropped {
    auto const database = std::make_shared<database_mock>();
    auto const module_pool = std::make_shared<ae::module_pool>(database.get());

    module_object const module1{db::make_temporary_id(), {"base", {10, 4}, 0, 0, ""}};

    module_pool->revert_modules_and_notify({module1});

    module_pool->overwrite_module({"overwrite", {11, 2}, 0, 100, ""});

    auto const &modules = module_pool->modules();
    XCTAssertEqual(modules.size(), 3);

    auto iterator = modules.begin();

    XCTAssertEqual(iterator->first.range, (time::range{10, 1}));
    XCTAssertEqual(iterator->second.value.name, "base");
    XCTAssertEqual(iterator->second.value.range, (time::range{10, 1}));
    XCTAssertEqual(iterator->second.value.file_frame, 0);

    ++iterator;

    XCTAssertEqual(iterator->first.range, (time::range{11, 2}));
    XCTAssertEqual(iterator->second.value.name, "overwrite");
    XCTAssertEqual(iterator->second.value.range, (time::range{11, 2}));
    XCTAssertEqual(iterator->second.value.file_frame, 100);

    ++iterator;

    XCTAssertEqual(iterator->first.range, (time::range{13, 1}));
    XCTAssertEqual(iterator->second.value.name, "base");
    XCTAssertEqual(iterator->second.value.range, (time::range{13, 1}));
    XCTAssertEqual(iterator->second.value.file_frame, 3);
}

- (void)test_overwrite_module_edge_cropped {
    auto const database = std::make_shared<database_mock>();
    auto const module_pool = std::make_shared<ae::module_pool>(database.get());

    module_object const module1{db::make_temporary_id(), {"base_1", {100, 3}, 0, 0, ""}};
    module_object const module2{db::make_temporary_id(), {"base_2", {103, 3}, 0, 3, ""}};

    module_pool->revert_modules_and_notify({module1, module2});

    module_pool->overwrite_module({"overwrite", {102, 2}, 0, 200, ""});

    auto const &modules = module_pool->modules();
    XCTAssertEqual(modules.size(), 3);

    auto iterator = modules.begin();

    XCTAssertEqual(iterator->first.range, (time::range{100, 2}));
    XCTAssertEqual(iterator->second.value.name, "base_1");
    XCTAssertEqual(iterator->second.value.range, (time::range{100, 2}));
    XCTAssertEqual(iterator->second.value.file_frame, 0);

    ++iterator;

    XCTAssertEqual(iterator->first.range, (time::range{102, 2}));
    XCTAssertEqual(iterator->second.value.name, "overwrite");
    XCTAssertEqual(iterator->second.value.range, (time::range{102, 2}));
    XCTAssertEqual(iterator->second.value.file_frame, 200);

    ++iterator;

    XCTAssertEqual(iterator->first.range, (time::range{104, 2}));
    XCTAssertEqual(iterator->second.value.name, "base_2");
    XCTAssertEqual(iterator->second.value.range, (time::range{104, 2}));
    XCTAssertEqual(iterator->second.value.file_frame, 4);
}

@end

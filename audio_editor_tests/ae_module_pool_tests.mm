//
//  ae_module_pool_tests.mm
//

#import <XCTest/XCTest.h>
#import <audio_editor_core/ae_db_constants.h>
#import <audio_editor_core/ae_db_module.h>
#import <audio_editor_core/ae_db_utils.h>
#import <audio_editor_core/ae_module_pool.h>

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
        return db_module{object};
    }

    void remove_module(object_id const &) override {
    }

    void update_module(object_id const &, module const &) override {
    }
};
}

using namespace yas::ae::module_pool_test_utils;

@interface ae_module_pool_tests : XCTestCase

@end

@implementation ae_module_pool_tests

- (void)test_insert_module_and_notify {
    auto const database = std::make_shared<database_mock>();
    auto const module_pool = std::make_shared<ae::module_pool>(database.get());

    XCTAssertEqual(module_pool->modules().size(), 0);

    module const module_params_1{"", {0, 4}, 0, ""};

    auto const module1_index = module_pool->insert_module_and_notify(module_params_1);

    XCTAssertEqual(module_pool->modules().size(), 1);
    XCTAssertEqual(module_pool->module_at(module1_index.value()).value().index(), module1_index.value());

    module const module_params_2{"", {4, 3}, 4, ""};

    auto const module2_index = module_pool->insert_module_and_notify(module_params_2);

    XCTAssertEqual(module_pool->modules().size(), 2);
    XCTAssertEqual(module_pool->module_at(module2_index.value()).value().index(), module2_index.value());

    module const module_params_3{"", {-2, 2}, 7, ""};

    auto const module3_index = module_pool->insert_module_and_notify(module_params_3);

    XCTAssertEqual(module_pool->modules().size(), 3);
    XCTAssertEqual(module_pool->module_at(module3_index.value()).value().index(), module3_index.value());

    auto iterator = module_pool->modules().begin();
    XCTAssertEqual(iterator->first, module3_index);
    ++iterator;
    XCTAssertEqual(iterator->first, module1_index);
    ++iterator;
    XCTAssertEqual(iterator->first, module2_index);
}

- (void)test_erase_module_and_notify {
    auto const database = std::make_shared<database_mock>();
    auto const module_pool = std::make_shared<ae::module_pool>(database.get());

    module_object const module1{db::make_temporary_id(), {"module_1", {0, 4}, 0, ""}};
    module_object const module2{db::make_temporary_id(), {"module_1", {5, 3}, 5, ""}};

    module_pool->revert_modules_and_notify({module1, module2});

    XCTAssertEqual(module_pool->modules().size(), 2);

    module_pool->erase_module_and_notify(module1.index());

    XCTAssertEqual(module_pool->modules().size(), 1);
    XCTAssertEqual(module_pool->modules().count(module2.index()), 1);
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

    module_object const module1{db::make_temporary_id(), {"module_1", {0, 4}, 0, ""}};
    module_object const module2{db::make_temporary_id(), {"module_2", {7, 5}, 7, ""}};

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

    auto const module3_index = module_pool->insert_module_and_notify({"module_3", {4, 3}, 4, ""});

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

- (void)test_module_at_frame {
    auto const database = std::make_shared<database_mock>();
    auto const module_pool = std::make_shared<ae::module_pool>(database.get());

    module_object const module1{db::make_temporary_id(), {"module_1", {0, 1}, 0, ""}};
    module_object const module2{db::make_temporary_id(), {"module_2", {1, 2}, 1, ""}};
    module_object const module3{db::make_temporary_id(), {"module_3", {4, 3}, 4, ""}};

    module_pool->revert_modules_and_notify({module1, module2, module3});

    XCTAssertEqual(module_pool->module_at(-1), std::nullopt);
    XCTAssertEqual(module_pool->module_at(0).value().value.name, "module_1");
    XCTAssertEqual(module_pool->module_at(0).value().value.range, (time::range{0, 1}));
    XCTAssertEqual(module_pool->module_at(0).value().value.file_frame, 0);
    XCTAssertEqual(module_pool->module_at(1).value().value.name, "module_2");
    XCTAssertEqual(module_pool->module_at(1).value().value.range, (time::range{1, 2}));
    XCTAssertEqual(module_pool->module_at(1).value().value.file_frame, 1);
    XCTAssertEqual(module_pool->module_at(2).value().value.name, "module_2");
    XCTAssertEqual(module_pool->module_at(2).value().value.range, (time::range{1, 2}));
    XCTAssertEqual(module_pool->module_at(2).value().value.file_frame, 1);
    XCTAssertEqual(module_pool->module_at(3), std::nullopt);
    XCTAssertEqual(module_pool->module_at(4).value().value.name, "module_3");
    XCTAssertEqual(module_pool->module_at(4).value().value.range, (time::range{4, 3}));
    XCTAssertEqual(module_pool->module_at(4).value().value.file_frame, 4);
    XCTAssertEqual(module_pool->module_at(5).value().value.name, "module_3");
    XCTAssertEqual(module_pool->module_at(5).value().value.range, (time::range{4, 3}));
    XCTAssertEqual(module_pool->module_at(5).value().value.file_frame, 4);
    XCTAssertEqual(module_pool->module_at(6).value().value.name, "module_3");
    XCTAssertEqual(module_pool->module_at(6).value().value.range, (time::range{4, 3}));
    XCTAssertEqual(module_pool->module_at(6).value().value.file_frame, 4);
    XCTAssertEqual(module_pool->module_at(7), std::nullopt);
}

- (void)test_module_at_index {
    auto const database = std::make_shared<database_mock>();
    auto const module_pool = std::make_shared<ae::module_pool>(database.get());

    module_object const module1{db::make_temporary_id(), {"module_1", {0, 1}, 0, ""}};
    module_object const module2{db::make_temporary_id(), {"module_2", {1, 2}, 1, ""}};
    module_object const other_module{db::make_temporary_id(), {"other_module", {100, 100}, 100, ""}};

    module_pool->revert_modules_and_notify({module1, module2});

    XCTAssertEqual(module_pool->module_at(other_module.index()), std::nullopt);

    auto const stored_module1 = module_pool->module_at(module1.index());
    XCTAssertEqual(stored_module1.value().value.name, "module_1");
    auto const stored_module2 = module_pool->module_at(module2.index());
    XCTAssertEqual(stored_module2.value().value.name, "module_2");
}

- (void)test_previous_module_at {
    auto const database = std::make_shared<database_mock>();
    auto const module_pool = std::make_shared<ae::module_pool>(database.get());

    module_object const module1{db::make_temporary_id(), {"module_1", {0, 1}, 0, ""}};
    module_object const module2{db::make_temporary_id(), {"module_2", {1, 2}, 1, ""}};
    module_object const module3{db::make_temporary_id(), {"module_3", {4, 3}, 4, ""}};

    module_pool->revert_modules_and_notify({module1, module2, module3});

    XCTAssertEqual(module_pool->previous_module_at(-1), std::nullopt);
    XCTAssertEqual(module_pool->previous_module_at(0), std::nullopt);
    XCTAssertEqual(module_pool->previous_module_at(1).value().value.name, "module_1");
    XCTAssertEqual(module_pool->previous_module_at(1).value().value.range, (time::range{0, 1}));
    XCTAssertEqual(module_pool->previous_module_at(1).value().value.file_frame, 0);
    XCTAssertEqual(module_pool->previous_module_at(2).value().value.name, "module_1");
    XCTAssertEqual(module_pool->previous_module_at(2).value().value.range, (time::range{0, 1}));
    XCTAssertEqual(module_pool->previous_module_at(2).value().value.file_frame, 0);
    XCTAssertEqual(module_pool->previous_module_at(3).value().value.name, "module_2");
    XCTAssertEqual(module_pool->previous_module_at(3).value().value.range, (time::range{1, 2}));
    XCTAssertEqual(module_pool->previous_module_at(3).value().value.file_frame, 1);
    XCTAssertEqual(module_pool->previous_module_at(4).value().value.name, "module_2");
    XCTAssertEqual(module_pool->previous_module_at(4).value().value.range, (time::range{1, 2}));
    XCTAssertEqual(module_pool->previous_module_at(4).value().value.file_frame, 1);
    XCTAssertEqual(module_pool->previous_module_at(5).value().value.name, "module_2");
    XCTAssertEqual(module_pool->previous_module_at(5).value().value.range, (time::range{1, 2}));
    XCTAssertEqual(module_pool->previous_module_at(5).value().value.file_frame, 1);
    XCTAssertEqual(module_pool->previous_module_at(6).value().value.name, "module_2");
    XCTAssertEqual(module_pool->previous_module_at(6).value().value.range, (time::range{1, 2}));
    XCTAssertEqual(module_pool->previous_module_at(6).value().value.file_frame, 1);
    XCTAssertEqual(module_pool->previous_module_at(7).value().value.name, "module_3");
    XCTAssertEqual(module_pool->previous_module_at(7).value().value.range, (time::range{4, 3}));
    XCTAssertEqual(module_pool->previous_module_at(7).value().value.file_frame, 4);
}

- (void)test_next_module_at {
    auto const database = std::make_shared<database_mock>();
    auto const module_pool = std::make_shared<ae::module_pool>(database.get());

    module_object const module1{db::make_temporary_id(), {"module_1", {0, 1}, 0, ""}};
    module_object const module2{db::make_temporary_id(), {"module_2", {1, 2}, 1, ""}};
    module_object const module3{db::make_temporary_id(), {"module_3", {4, 3}, 4, ""}};

    module_pool->revert_modules_and_notify({module1, module2, module3});

    XCTAssertEqual(module_pool->next_module_at(-1).value().value.name, "module_1");
    XCTAssertEqual(module_pool->next_module_at(-1).value().value.range, (time::range{0, 1}));
    XCTAssertEqual(module_pool->next_module_at(-1).value().value.file_frame, 0);
    XCTAssertEqual(module_pool->next_module_at(0).value().value.name, "module_2");
    XCTAssertEqual(module_pool->next_module_at(0).value().value.range, (time::range{1, 2}));
    XCTAssertEqual(module_pool->next_module_at(0).value().value.file_frame, 1);
    XCTAssertEqual(module_pool->next_module_at(1).value().value.name, "module_3");
    XCTAssertEqual(module_pool->next_module_at(1).value().value.range, (time::range{4, 3}));
    XCTAssertEqual(module_pool->next_module_at(1).value().value.file_frame, 4);
    XCTAssertEqual(module_pool->next_module_at(2).value().value.name, "module_3");
    XCTAssertEqual(module_pool->next_module_at(2).value().value.range, (time::range{4, 3}));
    XCTAssertEqual(module_pool->next_module_at(2).value().value.file_frame, 4);
    XCTAssertEqual(module_pool->next_module_at(3).value().value.name, "module_3");
    XCTAssertEqual(module_pool->next_module_at(3).value().value.range, (time::range{4, 3}));
    XCTAssertEqual(module_pool->next_module_at(3).value().value.file_frame, 4);
    XCTAssertEqual(module_pool->next_module_at(4), std::nullopt);
    XCTAssertEqual(module_pool->next_module_at(5), std::nullopt);
    XCTAssertEqual(module_pool->next_module_at(6), std::nullopt);
    XCTAssertEqual(module_pool->next_module_at(7), std::nullopt);
}

- (void)test_splittable_module_at {
    auto const database = std::make_shared<database_mock>();
    auto const module_pool = std::make_shared<ae::module_pool>(database.get());

    module_object const module1{db::make_temporary_id(), {"", {0, 1}, 0, ""}};
    module_object const module2{db::make_temporary_id(), {"", {1, 2}, 1, ""}};
    module_object const module3{db::make_temporary_id(), {"", {3, 3}, 3, ""}};
    module_object const module4{db::make_temporary_id(), {"", {7, 2}, 7, ""}};

    module_pool->revert_modules_and_notify({module1, module2, module3, module4});

    XCTAssertFalse(module_pool->splittable_module_at(-1));
    XCTAssertFalse(module_pool->splittable_module_at(0));
    XCTAssertFalse(module_pool->splittable_module_at(1));
    XCTAssertTrue(module_pool->splittable_module_at(2));
    XCTAssertFalse(module_pool->splittable_module_at(3));
    XCTAssertTrue(module_pool->splittable_module_at(4));
    XCTAssertTrue(module_pool->splittable_module_at(5));
    XCTAssertFalse(module_pool->splittable_module_at(6));
    XCTAssertFalse(module_pool->splittable_module_at(7));
    XCTAssertTrue(module_pool->splittable_module_at(8));
    XCTAssertFalse(module_pool->splittable_module_at(9));
}

- (void)test_split_at {
    auto const database = std::make_shared<database_mock>();
    auto const module_pool = std::make_shared<ae::module_pool>(database.get());

    module_object const src_module{db::make_temporary_id(), {"split_module_name", {0, 8}, 0, ""}};

    module_pool->revert_modules_and_notify({src_module});

    module_pool->split_at(-1);
    module_pool->split_at(0);
    module_pool->split_at(8);
    module_pool->split_at(9);

    XCTAssertEqual(module_pool->modules().size(), 1);

    {
        module_pool->split_at(1);

        XCTAssertEqual(module_pool->modules().size(), 2);

        auto iterator = module_pool->modules().begin();

        XCTAssertEqual(iterator->first.range, (time::range{0, 1}));
        XCTAssertEqual(iterator->second.value.name, "split_module_name");
        XCTAssertEqual(iterator->second.value.range, (time::range{0, 1}));
        XCTAssertEqual(iterator->second.value.file_frame, 0);

        ++iterator;

        XCTAssertEqual(iterator->first.range, (time::range{1, 7}));
        XCTAssertEqual(iterator->second.value.name, "split_module_name");
        XCTAssertEqual(iterator->second.value.range, (time::range{1, 7}));
        XCTAssertEqual(iterator->second.value.file_frame, 1);
    }

    {
        module_pool->split_at(3);

        XCTAssertEqual(module_pool->modules().size(), 3);

        auto iterator = module_pool->modules().begin();

        XCTAssertEqual(iterator->first.range, (time::range{0, 1}));
        XCTAssertEqual(iterator->second.value.name, "split_module_name");
        XCTAssertEqual(iterator->second.value.range, (time::range{0, 1}));
        XCTAssertEqual(iterator->second.value.file_frame, 0);

        ++iterator;

        XCTAssertEqual(iterator->first.range, (time::range{1, 2}));
        XCTAssertEqual(iterator->second.value.name, "split_module_name");
        XCTAssertEqual(iterator->second.value.range, (time::range{1, 2}));
        XCTAssertEqual(iterator->second.value.file_frame, 1);

        ++iterator;

        XCTAssertEqual(iterator->first.range, (time::range{3, 5}));
        XCTAssertEqual(iterator->second.value.name, "split_module_name");
        XCTAssertEqual(iterator->second.value.range, (time::range{3, 5}));
        XCTAssertEqual(iterator->second.value.file_frame, 3);
    }
}

- (void)test_erase_at {
    auto const database = std::make_shared<database_mock>();
    auto const module_pool = std::make_shared<ae::module_pool>(database.get());

    module_pool->revert_modules_and_notify({{db::make_temporary_id(), {"module_1", {0, 2}, 0, ""}},
                                            {db::make_temporary_id(), {"module_2", {2, 2}, 2, ""}},
                                            {db::make_temporary_id(), {"module_3", {4, 2}, 4, ""}}});

    XCTAssertEqual(module_pool->modules().size(), 3);

    module_pool->erase_at(3);

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

- (void)test_erase_and_offset_at {
    auto const database = std::make_shared<database_mock>();
    auto const module_pool = std::make_shared<ae::module_pool>(database.get());

    module_pool->revert_modules_and_notify({{db::make_temporary_id(), {"module_1", {0, 2}, 0, ""}},
                                            {db::make_temporary_id(), {"module_2", {2, 2}, 2, ""}},
                                            {db::make_temporary_id(), {"module_3", {4, 2}, 4, ""}}});

    XCTAssertEqual(module_pool->modules().size(), 3);

    module_pool->erase_and_offset_at(3);

    XCTAssertEqual(module_pool->modules().size(), 2);

    auto iterator = module_pool->modules().begin();

    XCTAssertEqual(iterator->first.range, (time::range{0, 2}));
    XCTAssertEqual(iterator->second.value.name, "module_1");
    XCTAssertEqual(iterator->second.value.range, (time::range{0, 2}));
    XCTAssertEqual(iterator->second.value.file_frame, 0);

    ++iterator;

    XCTAssertEqual(iterator->first.range, (time::range{2, 2}));
    XCTAssertEqual(iterator->second.value.name, "module_3");
    XCTAssertEqual(iterator->second.value.range, (time::range{2, 2}));
    XCTAssertEqual(iterator->second.value.file_frame, 4);
}

- (void)test_drop_head_at {
    auto const database = std::make_shared<database_mock>();
    auto const module_pool = std::make_shared<ae::module_pool>(database.get());

    module_object const src_module{db::make_temporary_id(), {"drop_head_module", {10, 4}, 100, ""}};

    module_pool->revert_modules_and_notify({src_module});

    module_pool->drop_head_at(9);
    module_pool->drop_head_at(10);
    module_pool->drop_head_at(14);
    module_pool->drop_head_at(15);

    XCTAssertEqual(module_pool->modules().size(), 1);
    XCTAssertEqual(module_pool->modules().begin()->first.range, (time::range{10, 4}));

    module_pool->drop_head_at(11);

    auto iterator = module_pool->modules().begin();

    XCTAssertEqual(module_pool->modules().size(), 1);
    XCTAssertEqual(iterator->first.range, (time::range{11, 3}));
    XCTAssertEqual(iterator->second.value.name, "drop_head_module");
    XCTAssertEqual(iterator->second.value.range, (time::range{11, 3}));
    XCTAssertEqual(iterator->second.value.file_frame, 101);
}

- (void)test_drop_tail_at {
    auto const database = std::make_shared<database_mock>();
    auto const module_pool = std::make_shared<ae::module_pool>(database.get());

    module_object const src_module{db::make_temporary_id(), {"drop_tail_module", {10, 4}, 100, ""}};

    module_pool->revert_modules_and_notify({src_module});

    module_pool->drop_tail_at(9);
    module_pool->drop_tail_at(10);
    module_pool->drop_tail_at(14);
    module_pool->drop_tail_at(15);

    XCTAssertEqual(module_pool->modules().size(), 1);
    XCTAssertEqual(module_pool->modules().begin()->first.range, (time::range{10, 4}));

    module_pool->drop_tail_at(13);

    auto iterator = module_pool->modules().begin();

    XCTAssertEqual(module_pool->modules().size(), 1);
    XCTAssertEqual(iterator->first.range, (time::range{10, 3}));
    XCTAssertEqual(iterator->second.value.name, "drop_tail_module");
    XCTAssertEqual(iterator->second.value.range, (time::range{10, 3}));
    XCTAssertEqual(iterator->second.value.file_frame, 100);
}

- (void)test_drop_head_and_offset_at {
    auto const database = std::make_shared<database_mock>();
    auto const module_pool = std::make_shared<ae::module_pool>(database.get());

    module_object const module1{db::make_temporary_id(), {"module_1", {0, 1}, 0, ""}};
    module_object const module2{db::make_temporary_id(), {"module_2", {1, 2}, 1, ""}};
    module_object const module3{db::make_temporary_id(), {"module_3", {4, 3}, 4, ""}};

    module_pool->revert_modules_and_notify({module1, module2, module3});

    module_pool->drop_head_and_offset_at(2);

    auto const &modules = module_pool->modules();
    XCTAssertEqual(modules.size(), 3);

    auto iterator = module_pool->modules().begin();

    XCTAssertEqual(iterator->first.range, (time::range{0, 1}));
    XCTAssertEqual(iterator->second.value.name, "module_1");
    XCTAssertEqual(iterator->second.value.range, (time::range{0, 1}));
    XCTAssertEqual(iterator->second.value.file_frame, 0);

    ++iterator;

    XCTAssertEqual(iterator->first.range, (time::range{1, 1}));
    XCTAssertEqual(iterator->second.value.name, "module_2");
    XCTAssertEqual(iterator->second.value.range, (time::range{1, 1}));
    XCTAssertEqual(iterator->second.value.file_frame, 2);

    ++iterator;

    XCTAssertEqual(iterator->first.range, (time::range{3, 3}));
    XCTAssertEqual(iterator->second.value.name, "module_3");
    XCTAssertEqual(iterator->second.value.range, (time::range{3, 3}));
    XCTAssertEqual(iterator->second.value.file_frame, 4);
}

- (void)test_drop_tail_and_offset {
    auto const database = std::make_shared<database_mock>();
    auto const module_pool = std::make_shared<ae::module_pool>(database.get());

    module_object const module1{db::make_temporary_id(), {"module_1", {0, 1}, 0, ""}};
    module_object const module2{db::make_temporary_id(), {"module_2", {1, 2}, 1, ""}};
    module_object const module3{db::make_temporary_id(), {"module_3", {4, 3}, 4, ""}};

    module_pool->revert_modules_and_notify({module1, module2, module3});

    module_pool->drop_tail_and_offset_at(2);

    auto const &modules = module_pool->modules();
    XCTAssertEqual(modules.size(), 3);

    auto iterator = modules.begin();

    XCTAssertEqual(iterator->first.range, (time::range{0, 1}));
    XCTAssertEqual(iterator->second.value.name, "module_1");
    XCTAssertEqual(iterator->second.value.range, (time::range{0, 1}));
    XCTAssertEqual(iterator->second.value.file_frame, 0);

    ++iterator;

    XCTAssertEqual(iterator->first.range, (time::range{1, 1}));
    XCTAssertEqual(iterator->second.value.name, "module_2");
    XCTAssertEqual(iterator->second.value.range, (time::range{1, 1}));
    XCTAssertEqual(iterator->second.value.file_frame, 1);

    ++iterator;

    XCTAssertEqual(iterator->first.range, (time::range{3, 3}));
    XCTAssertEqual(iterator->second.value.name, "module_3");
    XCTAssertEqual(iterator->second.value.range, (time::range{3, 3}));
    XCTAssertEqual(iterator->second.value.file_frame, 4);
}

- (void)test_overwrite_module_middle_cropped {
    auto const database = std::make_shared<database_mock>();
    auto const module_pool = std::make_shared<ae::module_pool>(database.get());

    module_object const module1{db::make_temporary_id(), {"base", {10, 4}, 0, ""}};

    module_pool->revert_modules_and_notify({module1});

    module_pool->overwrite_module({"overwrite", {11, 2}, 100, ""});

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

    module_object const module1{db::make_temporary_id(), {"base_1", {100, 3}, 0, ""}};
    module_object const module2{db::make_temporary_id(), {"base_2", {103, 3}, 3, ""}};

    module_pool->revert_modules_and_notify({module1, module2});

    module_pool->overwrite_module({"overwrite", {102, 2}, 200, ""});

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

- (void)test_move_one_module {
    auto const database = std::make_shared<database_mock>();
    auto const module_pool = std::make_shared<ae::module_pool>(database.get());

    module_object const module1{db::make_temporary_id(), {"move_module", {0, 1}, 0, ""}};

    module_pool->revert_modules_and_notify({module1});

    {
        module_pool->move_modules({module1.index()}, 1);

        XCTAssertEqual(module_pool->modules().size(), 1);

        auto iterator = module_pool->modules().begin();

        XCTAssertEqual(iterator->first.range, (time::range{1, 1}));
        XCTAssertEqual(iterator->second.value.name, "move_module");
        XCTAssertEqual(iterator->second.value.range, (time::range{1, 1}));
        XCTAssertEqual(iterator->second.value.file_frame, 0);
    }

    {
        module_pool->move_modules({module_pool->first_module()->index()}, -2);

        XCTAssertEqual(module_pool->modules().size(), 1);

        auto iterator = module_pool->modules().begin();

        XCTAssertEqual(iterator->first.range, (time::range{-1, 1}));
        XCTAssertEqual(iterator->second.value.name, "move_module");
        XCTAssertEqual(iterator->second.value.range, (time::range{-1, 1}));
        XCTAssertEqual(iterator->second.value.file_frame, 0);
    }
}

- (void)test_move_many_modules {
    auto const database = std::make_shared<database_mock>();
    auto const module_pool = std::make_shared<ae::module_pool>(database.get());

    module_object const module1{db::make_temporary_id(), {"module_1", {0, 1}, 0, ""}};
    module_object const module2{db::make_temporary_id(), {"module_2", {1, 1}, 1, ""}};
    module_object const module3{db::make_temporary_id(), {"module_3", {2, 1}, 2, ""}};

    module_pool->revert_modules_and_notify({module1, module2, module3});

    module_pool->move_modules({module2.index(), module3.index()}, 1);

    XCTAssertEqual(module_pool->modules().size(), 3);

    auto iterator = module_pool->modules().begin();

    XCTAssertEqual(iterator->first.range, (time::range{0, 1}));
    XCTAssertEqual(iterator->second.value.name, "module_1");
    XCTAssertEqual(iterator->second.value.range, (time::range{0, 1}));
    XCTAssertEqual(iterator->second.value.file_frame, 0);

    ++iterator;

    XCTAssertEqual(iterator->first.range, (time::range{2, 1}));
    XCTAssertEqual(iterator->second.value.name, "module_2");
    XCTAssertEqual(iterator->second.value.range, (time::range{2, 1}));
    XCTAssertEqual(iterator->second.value.file_frame, 1);

    ++iterator;

    XCTAssertEqual(iterator->first.range, (time::range{3, 1}));
    XCTAssertEqual(iterator->second.value.name, "module_3");
    XCTAssertEqual(iterator->second.value.range, (time::range{3, 1}));
    XCTAssertEqual(iterator->second.value.file_frame, 2);
}

- (void)test_move_cropped {
    auto const database = std::make_shared<database_mock>();
    auto const module_pool = std::make_shared<ae::module_pool>(database.get());

    module_object const module1{db::make_temporary_id(), {"module_1", {0, 4}, 0, ""}};
    module_object const module2{db::make_temporary_id(), {"module_2", {4, 2}, 4, ""}};

    module_pool->revert_modules_and_notify({module1, module2});

    XCTAssertEqual(module_pool->modules().size(), 2);

    module_pool->move_modules({module2.index()}, -3);

    XCTAssertEqual(module_pool->modules().size(), 3);

    auto iterator = module_pool->modules().begin();

    XCTAssertEqual(iterator->first.range, (time::range{0, 1}));
    XCTAssertEqual(iterator->second.value.name, "module_1");
    XCTAssertEqual(iterator->second.value.range, (time::range{0, 1}));
    XCTAssertEqual(iterator->second.value.file_frame, 0);

    ++iterator;

    XCTAssertEqual(iterator->first.range, (time::range{1, 2}));
    XCTAssertEqual(iterator->second.value.name, "module_2");
    XCTAssertEqual(iterator->second.value.range, (time::range{1, 2}));
    XCTAssertEqual(iterator->second.value.file_frame, 4);

    ++iterator;

    XCTAssertEqual(iterator->first.range, (time::range{3, 1}));
    XCTAssertEqual(iterator->second.value.name, "module_1");
    XCTAssertEqual(iterator->second.value.range, (time::range{3, 1}));
    XCTAssertEqual(iterator->second.value.file_frame, 3);
}

@end

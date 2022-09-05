//
//  ae_file_track_tests.mm
//

#import <XCTest/XCTest.h>
#import <audio_editor_core/ae_file_track.h>
#import <cpp_utils/yas_identifier.h>

using namespace yas;
using namespace yas::ae;

@interface ae_file_track_tests : XCTestCase

@end

@implementation ae_file_track_tests

- (void)test_insert_module_and_notify {
    auto const track = file_track::make_shared();

    XCTAssertEqual(track->modules().size(), 0);

    file_module const module1{{}, "", {0, 4}, 0, ""};

    track->insert_module_and_notify(module1);

    XCTAssertEqual(track->modules().size(), 1);
    XCTAssertEqual(track->modules().at(module1.range).identifier, module1.identifier);

    file_module const module2{{}, "", {4, 3}, 4, ""};

    track->insert_module_and_notify(module2);

    XCTAssertEqual(track->modules().size(), 2);
    XCTAssertEqual(track->modules().at(module2.range).identifier, module2.identifier);

    file_module const module3{{}, "", {-2, 2}, 7, ""};

    track->insert_module_and_notify(module3);

    XCTAssertEqual(track->modules().size(), 3);
    XCTAssertEqual(track->modules().at(module3.range).identifier, module3.identifier);

    auto iterator = track->modules().begin();
    XCTAssertEqual(iterator->first, module3.range);
    ++iterator;
    XCTAssertEqual(iterator->first, module1.range);
    ++iterator;
    XCTAssertEqual(iterator->first, module2.range);
}

- (void)test_erase_module_and_notify {
    auto const track = file_track::make_shared();

    file_module const module1{{}, "module_1", {0, 4}, 0, ""};
    file_module const module2{{}, "module_1", {5, 3}, 5, ""};

    track->revert_modules_and_notify({module1, module2});

    XCTAssertEqual(track->modules().size(), 2);

    track->erase_module_and_notify(module1);

    XCTAssertEqual(track->modules().size(), 1);
    XCTAssertEqual(track->modules().count(module2.range), 1);
}

- (void)test_observe_event {
    struct called_event {
        file_track_event_type type;
        std::optional<file_module> module{std::nullopt};
        file_track_module_map_t modules;
    };

    std::vector<called_event> called;

    auto const track = file_track::make_shared();

    file_module const module1{{}, "module_1", {0, 4}, 0, ""};
    file_module const module2{{}, "module_2", {7, 5}, 7, ""};

    track->revert_modules_and_notify({module1, module2});

    auto canceller =
        track
            ->observe_event([&called](auto const &event) {
                called.emplace_back(called_event{.type = event.type, .module = event.module, .modules = event.modules});
            })
            .sync();

    XCTAssertEqual(called.size(), 1);
    XCTAssertEqual(called.at(0).type, file_track_event_type::any);
    XCTAssertEqual(called.at(0).modules.size(), 2);
    XCTAssertEqual(called.at(0).module, std::nullopt);

    file_module const module3{{}, "module_3", {4, 3}, 4, ""};

    track->insert_module_and_notify(module3);

    XCTAssertEqual(called.size(), 2);
    XCTAssertEqual(called.at(1).type, file_track_event_type::inserted);
    XCTAssertEqual(called.at(1).modules.size(), 3);
    XCTAssertEqual(called.at(1).module.value().identifier, module3.identifier);

    track->erase_module_and_notify(module1);

    XCTAssertEqual(called.size(), 3);
    XCTAssertEqual(called.at(2).type, file_track_event_type::erased);
    XCTAssertEqual(called.at(2).modules.size(), 2);
    XCTAssertEqual(called.at(2).module.value().identifier, module1.identifier);

    track->revert_modules_and_notify({module2});

    XCTAssertEqual(called.size(), 4);
    XCTAssertEqual(called.at(3).type, file_track_event_type::reverted);
    XCTAssertEqual(called.at(3).modules.size(), 1);
    XCTAssertEqual(called.at(3).module, std::nullopt);

    canceller->cancel();
}

- (void)test_module_at {
    auto const track = file_track::make_shared();

    file_module const module1{{}, "module_1", {0, 1}, 0, ""};
    file_module const module2{{}, "module_2", {1, 2}, 1, ""};
    file_module const module3{{}, "module_3", {4, 3}, 4, ""};

    track->revert_modules_and_notify({module1, module2, module3});

    XCTAssertEqual(track->module_at(-1), std::nullopt);
    XCTAssertTrue(track->module_at(0).value().is_equal_location(file_module{{}, "", {0, 1}, 0, ""}));
    XCTAssertEqual(track->module_at(0).value().name, "module_1");
    XCTAssertTrue(track->module_at(1).value().is_equal_location(file_module{{}, "", {1, 2}, 1, ""}));
    XCTAssertEqual(track->module_at(1).value().name, "module_2");
    XCTAssertTrue(track->module_at(2).value().is_equal_location(file_module{{}, "", {1, 2}, 1, ""}));
    XCTAssertEqual(track->module_at(2).value().name, "module_2");
    XCTAssertEqual(track->module_at(3), std::nullopt);
    XCTAssertTrue(track->module_at(4).value().is_equal_location(file_module{{}, "", {4, 3}, 4, ""}));
    XCTAssertEqual(track->module_at(4).value().name, "module_3");
    XCTAssertTrue(track->module_at(5).value().is_equal_location(file_module{{}, "", {4, 3}, 4, ""}));
    XCTAssertEqual(track->module_at(5).value().name, "module_3");
    XCTAssertTrue(track->module_at(6).value().is_equal_location(file_module{{}, "", {4, 3}, 4, ""}));
    XCTAssertEqual(track->module_at(6).value().name, "module_3");
    XCTAssertEqual(track->module_at(7), std::nullopt);
}

- (void)test_previous_module_at {
    auto const track = file_track::make_shared();

    file_module const module1{{}, "module_1", {0, 1}, 0, ""};
    file_module const module2{{}, "module_2", {1, 2}, 1, ""};
    file_module const module3{{}, "module_3", {4, 3}, 4, ""};

    track->revert_modules_and_notify({module1, module2, module3});

    XCTAssertEqual(track->previous_module_at(-1), std::nullopt);
    XCTAssertEqual(track->previous_module_at(0), std::nullopt);
    XCTAssertTrue(track->previous_module_at(1).value().is_equal_location(file_module{{}, "", {0, 1}, 0, ""}));
    XCTAssertEqual(track->previous_module_at(1).value().name, "module_1");
    XCTAssertTrue(track->previous_module_at(2).value().is_equal_location(file_module{{}, "", {0, 1}, 0, ""}));
    XCTAssertEqual(track->previous_module_at(2).value().name, "module_1");
    XCTAssertTrue(track->previous_module_at(3).value().is_equal_location(file_module{{}, "", {1, 2}, 1, ""}));
    XCTAssertEqual(track->previous_module_at(3).value().name, "module_2");
    XCTAssertTrue(track->previous_module_at(4).value().is_equal_location(file_module{{}, "", {1, 2}, 1, ""}));
    XCTAssertEqual(track->previous_module_at(4).value().name, "module_2");
    XCTAssertTrue(track->previous_module_at(5).value().is_equal_location(file_module{{}, "", {1, 2}, 1, ""}));
    XCTAssertEqual(track->previous_module_at(5).value().name, "module_2");
    XCTAssertTrue(track->previous_module_at(6).value().is_equal_location(file_module{{}, "", {1, 2}, 1, ""}));
    XCTAssertEqual(track->previous_module_at(6).value().name, "module_2");
    XCTAssertTrue(track->previous_module_at(7).value().is_equal_location(file_module{{}, "", {4, 3}, 4, ""}));
    XCTAssertEqual(track->previous_module_at(7).value().name, "module_3");
}

- (void)test_next_module_at {
    auto const track = file_track::make_shared();

    file_module const module1{{}, "module_1", {0, 1}, 0, ""};
    file_module const module2{{}, "module_2", {1, 2}, 1, ""};
    file_module const module3{{}, "module_3", {4, 3}, 4, ""};

    track->revert_modules_and_notify({module1, module2, module3});

    XCTAssertTrue(track->next_module_at(-1).value().is_equal_location(file_module{{}, "", {0, 1}, 0, ""}));
    XCTAssertEqual(track->next_module_at(-1).value().name, "module_1");
    XCTAssertTrue(track->next_module_at(0).value().is_equal_location(file_module{{}, "", {1, 2}, 1, ""}));
    XCTAssertEqual(track->next_module_at(0).value().name, "module_2");
    XCTAssertTrue(track->next_module_at(1).value().is_equal_location(file_module{{}, "", {4, 3}, 4, ""}));
    XCTAssertEqual(track->next_module_at(1).value().name, "module_3");
    XCTAssertTrue(track->next_module_at(2).value().is_equal_location(file_module{{}, "", {4, 3}, 4, ""}));
    XCTAssertEqual(track->next_module_at(2).value().name, "module_3");
    XCTAssertTrue(track->next_module_at(3).value().is_equal_location(file_module{{}, "", {4, 3}, 4, ""}));
    XCTAssertEqual(track->next_module_at(3).value().name, "module_3");
    XCTAssertEqual(track->next_module_at(4), std::nullopt);
    XCTAssertEqual(track->next_module_at(5), std::nullopt);
    XCTAssertEqual(track->next_module_at(6), std::nullopt);
    XCTAssertEqual(track->next_module_at(7), std::nullopt);
}

- (void)test_splittable_module_at {
    auto const track = file_track::make_shared();

    file_module const module1{{}, "", {0, 1}, 0, ""};
    file_module const module2{{}, "", {1, 2}, 1, ""};
    file_module const module3{{}, "", {3, 3}, 3, ""};
    file_module const module4{{}, "", {7, 2}, 7, ""};

    track->revert_modules_and_notify({module1, module2, module3, module4});

    XCTAssertFalse(track->splittable_module_at(-1));
    XCTAssertFalse(track->splittable_module_at(0));
    XCTAssertFalse(track->splittable_module_at(1));
    XCTAssertTrue(track->splittable_module_at(2));
    XCTAssertFalse(track->splittable_module_at(3));
    XCTAssertTrue(track->splittable_module_at(4));
    XCTAssertTrue(track->splittable_module_at(5));
    XCTAssertFalse(track->splittable_module_at(6));
    XCTAssertFalse(track->splittable_module_at(7));
    XCTAssertTrue(track->splittable_module_at(8));
    XCTAssertFalse(track->splittable_module_at(9));
}

- (void)test_split_at {
    auto const track = file_track::make_shared();

    file_module const src_module{{}, "split_module_name", {0, 8}, 0, ""};

    track->revert_modules_and_notify({src_module});

    track->split_at(-1);
    track->split_at(0);
    track->split_at(8);
    track->split_at(9);

    XCTAssertEqual(track->modules().size(), 1);

    track->split_at(1);

    XCTAssertEqual(track->modules().size(), 2);
    XCTAssertEqual(track->modules().count(time::range{0, 1}), 1);
    XCTAssertTrue(track->modules().at(time::range{0, 1}).is_equal_location(file_module{{}, "", {0, 1}, 0, ""}));
    XCTAssertEqual(track->modules().at(time::range{0, 1}).name, "split_module_name");
    XCTAssertEqual(track->modules().count(time::range{1, 7}), 1);
    XCTAssertTrue(track->modules().at(time::range{1, 7}).is_equal_location(file_module{{}, "", {1, 7}, 1, ""}));
    XCTAssertEqual(track->modules().at(time::range{1, 7}).name, "split_module_name");

    track->split_at(3);

    XCTAssertEqual(track->modules().size(), 3);
    XCTAssertEqual(track->modules().count(time::range{0, 1}), 1);
    XCTAssertTrue(track->modules().at(time::range{0, 1}).is_equal_location(file_module{{}, "", {0, 1}, 0, ""}));
    XCTAssertEqual(track->modules().at(time::range{0, 1}).name, "split_module_name");
    XCTAssertEqual(track->modules().count(time::range{1, 2}), 1);
    XCTAssertTrue(track->modules().at(time::range{1, 2}).is_equal_location(file_module{{}, "", {1, 2}, 1, ""}));
    XCTAssertEqual(track->modules().at(time::range{1, 2}).name, "split_module_name");
    XCTAssertEqual(track->modules().count(time::range{3, 5}), 1);
    XCTAssertTrue(track->modules().at(time::range{3, 5}).is_equal_location(file_module{{}, "", {3, 5}, 3, ""}));
    XCTAssertEqual(track->modules().at(time::range{3, 5}).name, "split_module_name");
}

- (void)test_erase_at {
    auto const track = file_track::make_shared();

    track->revert_modules_and_notify(
        {{{}, "module_1", {0, 2}, 0, ""}, {{}, "module_2", {2, 2}, 2, ""}, {{}, "module_3", {4, 2}, 4, ""}});

    XCTAssertEqual(track->modules().size(), 3);

    track->erase_at(3);

    XCTAssertEqual(track->modules().size(), 2);
    XCTAssertEqual(track->modules().count(time::range{0, 2}), 1);
    XCTAssertTrue(track->modules().at(time::range{0, 2}).is_equal_location(file_module{{}, "", {0, 2}, 0, ""}));
    XCTAssertEqual(track->modules().at(time::range{0, 2}).name, "module_1");
    XCTAssertEqual(track->modules().count(time::range{4, 2}), 1);
    XCTAssertTrue(track->modules().at(time::range{4, 2}).is_equal_location(file_module{{}, "", {4, 2}, 4, ""}));
    XCTAssertEqual(track->modules().at(time::range{4, 2}).name, "module_3");
}

- (void)test_erase_and_offset_at {
    auto const track = file_track::make_shared();

    track->revert_modules_and_notify(
        {{{}, "module_1", {0, 2}, 0, ""}, {{}, "module_2", {2, 2}, 2, ""}, {{}, "module_3", {4, 2}, 4, ""}});

    XCTAssertEqual(track->modules().size(), 3);

    track->erase_and_offset_at(3);

    XCTAssertEqual(track->modules().size(), 2);
    XCTAssertEqual(track->modules().count(time::range{0, 2}), 1);
    XCTAssertTrue(track->modules().at(time::range{0, 2}).is_equal_location(file_module{{}, "", {0, 2}, 0, ""}));
    XCTAssertEqual(track->modules().at(time::range{0, 2}).name, "module_1");
    XCTAssertEqual(track->modules().count(time::range{2, 2}), 1);
    XCTAssertTrue(track->modules().at(time::range{2, 2}).is_equal_location(file_module{{}, "", {2, 2}, 4, ""}));
    XCTAssertEqual(track->modules().at(time::range{2, 2}).name, "module_3");
}

- (void)test_drop_head_at {
    auto const track = file_track::make_shared();

    file_module const src_module{{}, "drop_head_module", {10, 4}, 100, ""};

    track->revert_modules_and_notify({src_module});

    track->drop_head_at(9);
    track->drop_head_at(10);
    track->drop_head_at(14);
    track->drop_head_at(15);

    XCTAssertEqual(track->modules().size(), 1);
    XCTAssertEqual(track->modules().count(time::range{10, 4}), 1);

    track->drop_head_at(11);

    XCTAssertEqual(track->modules().size(), 1);
    XCTAssertEqual(track->modules().count(time::range{11, 3}), 1);
    XCTAssertTrue(track->modules().at(time::range{11, 3}).is_equal_location(file_module{{}, "", {11, 3}, 101, ""}));
    XCTAssertEqual(track->modules().at(time::range{11, 3}).name, "drop_head_module");
}

- (void)test_drop_tail_at {
    auto const track = file_track::make_shared();

    file_module const src_module{{}, "drop_tail_module", {10, 4}, 100, ""};

    track->revert_modules_and_notify({src_module});

    track->drop_tail_at(9);
    track->drop_tail_at(10);
    track->drop_tail_at(14);
    track->drop_tail_at(15);

    XCTAssertEqual(track->modules().size(), 1);
    XCTAssertEqual(track->modules().count(time::range{10, 4}), 1);

    track->drop_tail_at(13);

    XCTAssertEqual(track->modules().size(), 1);
    XCTAssertEqual(track->modules().count(time::range{10, 3}), 1);
    XCTAssertTrue(track->modules().at(time::range{10, 3}).is_equal_location(file_module{{}, "", {10, 3}, 100, ""}));
    XCTAssertEqual(track->modules().at(time::range{10, 3}).name, "drop_tail_module");
}

- (void)test_drop_head_and_offset_at {
    auto const track = file_track::make_shared();

    file_module const module1{{}, "module_1", {0, 1}, 0, ""};
    file_module const module2{{}, "module_2", {1, 2}, 1, ""};
    file_module const module3{{}, "module_3", {4, 3}, 4, ""};

    track->revert_modules_and_notify({module1, module2, module3});

    track->drop_head_and_offset_at(2);

    auto const &modules = track->modules();
    XCTAssertEqual(modules.size(), 3);
    XCTAssertTrue(modules.at(time::range{0, 1}).is_equal_location(file_module{{}, "", {0, 1}, 0, ""}));
    XCTAssertEqual(modules.at(time::range{0, 1}).name, "module_1");
    XCTAssertTrue(modules.at(time::range{1, 1}).is_equal_location(file_module{{}, "", {1, 1}, 2, ""}));
    XCTAssertEqual(modules.at(time::range{1, 1}).name, "module_2");
    XCTAssertTrue(modules.at(time::range{3, 3}).is_equal_location(file_module{{}, "", {3, 3}, 4, ""}));
    XCTAssertEqual(modules.at(time::range{3, 3}).name, "module_3");
}

- (void)test_drop_tail_and_offset {
    auto const track = file_track::make_shared();

    file_module const module1{{}, "module_1", {0, 1}, 0, ""};
    file_module const module2{{}, "module_2", {1, 2}, 1, ""};
    file_module const module3{{}, "module_3", {4, 3}, 4, ""};

    track->revert_modules_and_notify({module1, module2, module3});

    track->drop_tail_and_offset_at(2);

    auto const &modules = track->modules();
    XCTAssertEqual(modules.size(), 3);
    XCTAssertTrue(modules.at(time::range{0, 1}).is_equal_location(file_module{{}, "", {0, 1}, 0, ""}));
    XCTAssertEqual(modules.at(time::range{0, 1}).name, "module_1");
    XCTAssertTrue(modules.at(time::range{1, 1}).is_equal_location(file_module{{}, "", {1, 1}, 1, ""}));
    XCTAssertEqual(modules.at(time::range{1, 1}).name, "module_2");
    XCTAssertTrue(modules.at(time::range{3, 3}).is_equal_location(file_module{{}, "", {3, 3}, 4, ""}));
    XCTAssertEqual(modules.at(time::range{3, 3}).name, "module_3");
}

- (void)test_overwrite_module_middle_cropped {
    auto const track = file_track::make_shared();

    file_module const module1{{}, "base", {10, 4}, 0, ""};

    track->revert_modules_and_notify({module1});

    track->overwrite_module({{}, "overwrite", {11, 2}, 100, ""});

    auto const &modules = track->modules();
    XCTAssertEqual(modules.size(), 3);
    XCTAssertTrue(modules.at(time::range{10, 1}).is_equal_location(file_module{{}, "", {10, 1}, 0, ""}));
    XCTAssertEqual(modules.at(time::range{10, 1}).name, "base");
    XCTAssertTrue(modules.at(time::range{11, 2}).is_equal_location(file_module{{}, "", {11, 2}, 100, ""}));
    XCTAssertEqual(modules.at(time::range{11, 2}).name, "overwrite");
    XCTAssertTrue(modules.at(time::range{13, 1}).is_equal_location(file_module{{}, "", {13, 1}, 3, ""}));
    XCTAssertEqual(modules.at(time::range{13, 1}).name, "base");
}

- (void)test_overwrite_module_edge_cropped {
    auto const track = file_track::make_shared();

    file_module const module1{{}, "base_1", {100, 3}, 0, ""};
    file_module const module2{{}, "base_2", {103, 3}, 3, ""};

    track->revert_modules_and_notify({module1, module2});

    track->overwrite_module({{}, "overwrite", {102, 2}, 200, ""});

    auto const &modules = track->modules();
    XCTAssertEqual(modules.size(), 3);
    XCTAssertTrue(modules.at(time::range{100, 2}).is_equal_location(file_module{{}, "", {100, 2}, 0, ""}));
    XCTAssertEqual(modules.at(time::range{100, 2}).name, "base_1");
    XCTAssertTrue(modules.at(time::range{102, 2}).is_equal_location(file_module{{}, "", {102, 2}, 200, ""}));
    XCTAssertEqual(modules.at(time::range{102, 2}).name, "overwrite");
    XCTAssertTrue(modules.at(time::range{104, 2}).is_equal_location(file_module{{}, "", {104, 2}, 4, ""}));
    XCTAssertEqual(modules.at(time::range{104, 2}).name, "base_2");
}

- (void)test_move_one_module {
    auto const track = file_track::make_shared();

    file_module const module1{{}, "move_module", {0, 1}, 0, ""};

    track->revert_modules_and_notify({module1});

    track->move_modules({{0, 1}}, 1);

    XCTAssertEqual(track->modules().size(), 1);
    XCTAssertEqual(track->modules().count({1, 1}), 1);
    XCTAssertTrue(track->modules().at({1, 1}).is_equal_location(file_module{{}, "", {1, 1}, 0, ""}));
    XCTAssertEqual(track->modules().at({1, 1}).name, "move_module");

    track->move_modules({{1, 1}}, -2);

    XCTAssertEqual(track->modules().size(), 1);
    XCTAssertEqual(track->modules().count({-1, 1}), 1);
    XCTAssertTrue(track->modules().at({-1, 1}).is_equal_location(file_module{{}, "", {-1, 1}, 0, ""}));
    XCTAssertEqual(track->modules().at({-1, 1}).name, "move_module");
}

- (void)test_move_many_modules {
    auto const track = file_track::make_shared();

    file_module const module1{{}, "module_1", {0, 1}, 0, ""};
    file_module const module2{{}, "module_2", {1, 1}, 1, ""};
    file_module const module3{{}, "module_3", {2, 1}, 2, ""};

    track->revert_modules_and_notify({module1, module2, module3});

    track->move_modules({{1, 1}, {2, 1}}, 1);

    XCTAssertEqual(track->modules().size(), 3);
    XCTAssertEqual(track->modules().count({0, 1}), 1);
    XCTAssertTrue(track->modules().at({0, 1}).is_equal_location(file_module{{}, "", {0, 1}, 0, ""}));
    XCTAssertEqual(track->modules().at({0, 1}).name, "module_1");
    XCTAssertEqual(track->modules().count({2, 1}), 1);
    XCTAssertTrue(track->modules().at({2, 1}).is_equal_location(file_module{{}, "", {2, 1}, 1, ""}));
    XCTAssertEqual(track->modules().at({2, 1}).name, "module_2");
    XCTAssertEqual(track->modules().count({3, 1}), 1);
    XCTAssertTrue(track->modules().at({3, 1}).is_equal_location(file_module{{}, "", {3, 1}, 2, ""}));
    XCTAssertEqual(track->modules().at({3, 1}).name, "module_3");
}

- (void)test_move_cropped {
    auto const track = file_track::make_shared();

    file_module const module1{{}, "module_1", {0, 4}, 0, ""};
    file_module const module2{{}, "module_2", {4, 2}, 4, ""};

    track->revert_modules_and_notify({module1, module2});

    XCTAssertEqual(track->modules().size(), 2);

    track->move_modules({{4, 2}}, -3);

    XCTAssertEqual(track->modules().size(), 3);
    XCTAssertEqual(track->modules().count({0, 1}), 1);
    XCTAssertEqual(track->modules().count({1, 2}), 1);
    XCTAssertEqual(track->modules().count({3, 1}), 1);
    XCTAssertTrue(track->modules().at({0, 1}).is_equal_location(file_module{{}, "", {0, 1}, 0, ""}));
    XCTAssertEqual(track->modules().at({0, 1}).name, "module_1");
    XCTAssertTrue(track->modules().at({1, 2}).is_equal_location(file_module{{}, "", {1, 2}, 4, ""}));
    XCTAssertEqual(track->modules().at({1, 2}).name, "module_2");
    XCTAssertTrue(track->modules().at({3, 1}).is_equal_location(file_module{{}, "", {3, 1}, 3, ""}));
    XCTAssertEqual(track->modules().at({3, 1}).name, "module_1");
}

@end

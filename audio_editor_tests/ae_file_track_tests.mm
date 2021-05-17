//
//  ae_file_track_tests.mm
//

#import <XCTest/XCTest.h>
#import <audio_editor_core/audio_editor_core_umbrella.h>
#import <processing/yas_processing_umbrella.h>

using namespace yas;
using namespace yas::ae;

@interface ae_file_track_tests : XCTestCase

@end

@implementation ae_file_track_tests

- (void)test_insert_module {
    auto const track = file_track::make_shared();

    XCTAssertEqual(track->modules().size(), 0);

    file_module const module1{.range = {0, 4}, .file_frame = 0};

    track->insert_module_and_notify(module1);

    XCTAssertEqual(track->modules().size(), 1);
    XCTAssertEqual(track->modules().at(module1.range), module1);

    file_module const module2{.range = {4, 3}, .file_frame = 4};

    track->insert_module_and_notify(module2);

    XCTAssertEqual(track->modules().size(), 2);
    XCTAssertEqual(track->modules().at(module2.range), module2);

    file_module const module3{.range = {-2, 2}, .file_frame = 7};

    track->insert_module_and_notify(module3);

    XCTAssertEqual(track->modules().size(), 3);
    XCTAssertEqual(track->modules().at(module3.range), module3);

    auto iterator = track->modules().begin();
    XCTAssertEqual(iterator->first, module3.range);
    ++iterator;
    XCTAssertEqual(iterator->first, module1.range);
    ++iterator;
    XCTAssertEqual(iterator->first, module2.range);
}

- (void)test_erase_module {
    auto const track = file_track::make_shared();

    file_module const module1{.range = {0, 4}, .file_frame = 0};
    file_module const module2{.range = {5, 3}, .file_frame = 5};

    track->replace_modules_and_notify({module1, module2});

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

    file_module const module1{.range = {0, 4}, .file_frame = 0};
    file_module const module2{.range = {7, 5}, .file_frame = 7};

    track->replace_modules_and_notify({module1, module2});

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

    file_module const module3{.range = {4, 3}, .file_frame = 4};

    track->insert_module_and_notify(module3);

    XCTAssertEqual(called.size(), 2);
    XCTAssertEqual(called.at(1).type, file_track_event_type::inserted);
    XCTAssertEqual(called.at(1).modules.size(), 3);
    XCTAssertEqual(called.at(1).module, module3);

    track->erase_module_and_notify(module1);

    XCTAssertEqual(called.size(), 3);
    XCTAssertEqual(called.at(2).type, file_track_event_type::erased);
    XCTAssertEqual(called.at(2).modules.size(), 2);
    XCTAssertEqual(called.at(2).module, module1);

    canceller->cancel();
}

- (void)test_module {
    auto const track = file_track::make_shared();

    file_module const module1{.range = {0, 1}, .file_frame = 0};
    file_module const module2{.range = {1, 2}, .file_frame = 1};
    file_module const module3{.range = {4, 3}, .file_frame = 4};

    track->replace_modules_and_notify({module1, module2, module3});

    XCTAssertEqual(track->module_at(-1), std::nullopt);
    XCTAssertEqual(track->module_at(0), (file_module{.range = {0, 1}, .file_frame = 0}));
    XCTAssertEqual(track->module_at(1), (file_module{.range = {1, 2}, .file_frame = 1}));
    XCTAssertEqual(track->module_at(2), (file_module{.range = {1, 2}, .file_frame = 1}));
    XCTAssertEqual(track->module_at(3), std::nullopt);
    XCTAssertEqual(track->module_at(4), (file_module{.range = {4, 3}, .file_frame = 4}));
    XCTAssertEqual(track->module_at(5), (file_module{.range = {4, 3}, .file_frame = 4}));
    XCTAssertEqual(track->module_at(6), (file_module{.range = {4, 3}, .file_frame = 4}));
    XCTAssertEqual(track->module_at(7), std::nullopt);
}

- (void)test_previous_module {
    auto const track = file_track::make_shared();

    file_module const module1{.range = {0, 1}, .file_frame = 0};
    file_module const module2{.range = {1, 2}, .file_frame = 1};
    file_module const module3{.range = {4, 3}, .file_frame = 4};

    track->replace_modules_and_notify({module1, module2, module3});

    XCTAssertEqual(track->previous_module_at(-1), std::nullopt);
    XCTAssertEqual(track->previous_module_at(0), std::nullopt);
    XCTAssertEqual(track->previous_module_at(1), (file_module{.range = {0, 1}, .file_frame = 0}));
    XCTAssertEqual(track->previous_module_at(2), (file_module{.range = {0, 1}, .file_frame = 0}));
    XCTAssertEqual(track->previous_module_at(3), (file_module{.range = {1, 2}, .file_frame = 1}));
    XCTAssertEqual(track->previous_module_at(4), (file_module{.range = {1, 2}, .file_frame = 1}));
    XCTAssertEqual(track->previous_module_at(5), (file_module{.range = {1, 2}, .file_frame = 1}));
    XCTAssertEqual(track->previous_module_at(6), (file_module{.range = {1, 2}, .file_frame = 1}));
    XCTAssertEqual(track->previous_module_at(7), (file_module{.range = {4, 3}, .file_frame = 4}));
}

- (void)test_next_module {
    auto const track = file_track::make_shared();

    file_module const module1{.range = {0, 1}, .file_frame = 0};
    file_module const module2{.range = {1, 2}, .file_frame = 1};
    file_module const module3{.range = {4, 3}, .file_frame = 4};

    track->replace_modules_and_notify({module1, module2, module3});

    XCTAssertEqual(track->next_module_at(-1), (file_module{.range = {0, 1}, .file_frame = 0}));
    XCTAssertEqual(track->next_module_at(0), (file_module{.range = {1, 2}, .file_frame = 1}));
    XCTAssertEqual(track->next_module_at(1), (file_module{.range = {4, 3}, .file_frame = 4}));
    XCTAssertEqual(track->next_module_at(2), (file_module{.range = {4, 3}, .file_frame = 4}));
    XCTAssertEqual(track->next_module_at(3), (file_module{.range = {4, 3}, .file_frame = 4}));
    XCTAssertEqual(track->next_module_at(4), std::nullopt);
    XCTAssertEqual(track->next_module_at(5), std::nullopt);
    XCTAssertEqual(track->next_module_at(6), std::nullopt);
    XCTAssertEqual(track->next_module_at(7), std::nullopt);
}

- (void)test_splittable_module {
    auto const track = file_track::make_shared();

    file_module const module1{.range = {0, 1}, .file_frame = 0};
    file_module const module2{.range = {1, 2}, .file_frame = 1};
    file_module const module3{.range = {3, 3}, .file_frame = 3};
    file_module const module4{.range = {7, 2}, .file_frame = 7};

    track->replace_modules_and_notify({module1, module2, module3, module4});

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

- (void)test_split {
    auto const track = file_track::make_shared();

    file_module const src_module{.range = {0, 8}, .file_frame = 0};

    track->replace_modules_and_notify({src_module});

    track->split_at(-1);
    track->split_at(0);
    track->split_at(8);
    track->split_at(9);

    XCTAssertEqual(track->modules().size(), 1);

    track->split_at(1);

    XCTAssertEqual(track->modules().size(), 2);
    XCTAssertEqual(track->modules().count(proc::time::range{0, 1}), 1);
    XCTAssertEqual(track->modules().at(proc::time::range{0, 1}), (file_module{.range = {0, 1}, .file_frame = 0}));
    XCTAssertEqual(track->modules().count(proc::time::range{1, 7}), 1);
    XCTAssertEqual(track->modules().at(proc::time::range{1, 7}), (file_module{.range = {1, 7}, .file_frame = 1}));

    track->split_at(3);

    XCTAssertEqual(track->modules().size(), 3);
    XCTAssertEqual(track->modules().count(proc::time::range{0, 1}), 1);
    XCTAssertEqual(track->modules().at(proc::time::range{0, 1}), (file_module{.range = {0, 1}, .file_frame = 0}));
    XCTAssertEqual(track->modules().count(proc::time::range{1, 2}), 1);
    XCTAssertEqual(track->modules().at(proc::time::range{1, 2}), (file_module{.range = {1, 2}, .file_frame = 1}));
    XCTAssertEqual(track->modules().count(proc::time::range{3, 5}), 1);
    XCTAssertEqual(track->modules().at(proc::time::range{3, 5}), (file_module{.range = {3, 5}, .file_frame = 3}));
}

- (void)test_drop_head {
    auto const track = file_track::make_shared();

    file_module const src_module{.range = {10, 4}, .file_frame = 100};

    track->replace_modules_and_notify({src_module});

    track->drop_head_at(9);
    track->drop_head_at(10);
    track->drop_head_at(14);
    track->drop_head_at(15);

    XCTAssertEqual(track->modules().size(), 1);
    XCTAssertEqual(track->modules().count(proc::time::range{10, 4}), 1);

    track->drop_head_at(11);

    XCTAssertEqual(track->modules().size(), 1);
    XCTAssertEqual(track->modules().count(proc::time::range{11, 3}), 1);
    XCTAssertEqual(track->modules().at(proc::time::range{11, 3}), (file_module{.range = {11, 3}, .file_frame = 101}));
}

- (void)test_drop_tail {
    auto const track = file_track::make_shared();

    file_module const src_module{.range = {10, 4}, .file_frame = 100};

    track->replace_modules_and_notify({src_module});

    track->drop_tail(9);
    track->drop_tail(10);
    track->drop_tail(14);
    track->drop_tail(15);

    XCTAssertEqual(track->modules().size(), 1);
    XCTAssertEqual(track->modules().count(proc::time::range{10, 4}), 1);

    track->drop_tail(13);

    XCTAssertEqual(track->modules().size(), 1);
    XCTAssertEqual(track->modules().count(proc::time::range{10, 3}), 1);
    XCTAssertEqual(track->modules().at(proc::time::range{10, 3}), (file_module{.range = {10, 3}, .file_frame = 100}));
}

- (void)test_drop_head_and_offset {
    auto const track = file_track::make_shared();

    file_module const module1{.range = {0, 1}, .file_frame = 0};
    file_module const module2{.range = {1, 2}, .file_frame = 1};
    file_module const module3{.range = {4, 3}, .file_frame = 4};

    track->replace_modules_and_notify({module1, module2, module3});

    track->drop_head_and_offset(2);

    auto const &modules = track->modules();
    XCTAssertEqual(modules.size(), 3);
    XCTAssertEqual(modules.at(proc::time::range{0, 1}), (file_module{.range = {0, 1}, .file_frame = 0}));
    XCTAssertEqual(modules.at(proc::time::range{1, 1}), (file_module{.range = {1, 1}, .file_frame = 2}));
    XCTAssertEqual(modules.at(proc::time::range{3, 3}), (file_module{.range = {3, 3}, .file_frame = 4}));
}

- (void)test_drop_tail_and_offset {
    auto const track = file_track::make_shared();

    file_module const module1{.range = {0, 1}, .file_frame = 0};
    file_module const module2{.range = {1, 2}, .file_frame = 1};
    file_module const module3{.range = {4, 3}, .file_frame = 4};

    track->replace_modules_and_notify({module1, module2, module3});

    track->drop_tail_and_offset(2);

    auto const &modules = track->modules();
    XCTAssertEqual(modules.size(), 3);
    XCTAssertEqual(modules.at(proc::time::range{0, 1}), (file_module{.range = {0, 1}, .file_frame = 0}));
    XCTAssertEqual(modules.at(proc::time::range{1, 1}), (file_module{.range = {1, 1}, .file_frame = 1}));
    XCTAssertEqual(modules.at(proc::time::range{3, 3}), (file_module{.range = {3, 3}, .file_frame = 4}));
}

- (void)test_overwrite_module_middle_cropped {
    auto const track = file_track::make_shared();

    file_module const module1{.range = {0, 4}, .file_frame = 0};

    track->replace_modules_and_notify({module1});

    track->overwrite_module({.range = {1, 2}, .file_frame = 100});

    auto const &modules = track->modules();
    XCTAssertEqual(modules.size(), 3);
    XCTAssertEqual(modules.at(proc::time::range{0, 1}), (file_module{.range = {0, 1}, .file_frame = 0}));
    XCTAssertEqual(modules.at(proc::time::range{1, 2}), (file_module{.range = {1, 2}, .file_frame = 100}));
    XCTAssertEqual(modules.at(proc::time::range{3, 1}), (file_module{.range = {3, 1}, .file_frame = 3}));
}

- (void)test_overwrite_module_edge_cropped {
    auto const track = file_track::make_shared();

    file_module const module1{.range = {0, 3}, .file_frame = 0};
    file_module const module2{.range = {3, 3}, .file_frame = 3};

    track->replace_modules_and_notify({module1, module2});

    track->overwrite_module({.range = {2, 2}, .file_frame = 200});

    auto const &modules = track->modules();
    XCTAssertEqual(modules.size(), 3);
    XCTAssertEqual(modules.at(proc::time::range{0, 2}), (file_module{.range = {0, 2}, .file_frame = 0}));
    XCTAssertEqual(modules.at(proc::time::range{2, 2}), (file_module{.range = {2, 2}, .file_frame = 200}));
    XCTAssertEqual(modules.at(proc::time::range{4, 2}), (file_module{.range = {4, 2}, .file_frame = 4}));
}

- (void)test_move_one_module {
    auto const track = file_track::make_shared();

    file_module const module1{.range = {0, 1}, .file_frame = 0};

    track->replace_modules_and_notify({module1});

    track->move_modules({{0, 1}}, 1);

    XCTAssertEqual(track->modules().size(), 1);
    XCTAssertEqual(track->modules().count({1, 1}), 1);
    XCTAssertEqual(track->modules().at({1, 1}), (file_module{.range = {1, 1}, .file_frame = 0}));

    track->move_modules({{1, 1}}, -2);

    XCTAssertEqual(track->modules().size(), 1);
    XCTAssertEqual(track->modules().count({-1, 1}), 1);
    XCTAssertEqual(track->modules().at({-1, 1}), (file_module{.range = {-1, 1}, .file_frame = 0}));
}

- (void)test_move_many_modules {
    auto const track = file_track::make_shared();

    file_module const module1{.range = {0, 1}, .file_frame = 0};
    file_module const module2{.range = {1, 1}, .file_frame = 1};
    file_module const module3{.range = {2, 1}, .file_frame = 2};

    track->replace_modules_and_notify({module1, module2, module3});

    track->move_modules({{1, 1}, {2, 1}}, 1);

    XCTAssertEqual(track->modules().size(), 3);
    XCTAssertEqual(track->modules().count({0, 1}), 1);
    XCTAssertEqual(track->modules().at({0, 1}), (file_module{.range = {0, 1}, .file_frame = 0}));
    XCTAssertEqual(track->modules().count({2, 1}), 1);
    XCTAssertEqual(track->modules().at({2, 1}), (file_module{.range = {2, 1}, .file_frame = 1}));
    XCTAssertEqual(track->modules().count({3, 1}), 1);
    XCTAssertEqual(track->modules().at({3, 1}), (file_module{.range = {3, 1}, .file_frame = 2}));
}

- (void)test_move_cropped {
    auto const track = file_track::make_shared();

    file_module const module1{.range = {0, 4}, .file_frame = 0};
    file_module const module2{.range = {4, 2}, .file_frame = 4};

    track->replace_modules_and_notify({module1, module2});

    XCTAssertEqual(track->modules().size(), 2);

    track->move_modules({{4, 2}}, -3);

    XCTAssertEqual(track->modules().size(), 3);
    XCTAssertEqual(track->modules().count({0, 1}), 1);
    XCTAssertEqual(track->modules().count({1, 2}), 1);
    XCTAssertEqual(track->modules().count({3, 1}), 1);
    XCTAssertEqual(track->modules().at({0, 1}), (file_module{.range = {0, 1}, .file_frame = 0}));
    XCTAssertEqual(track->modules().at({1, 2}), (file_module{.range = {1, 2}, .file_frame = 4}));
    XCTAssertEqual(track->modules().at({3, 1}), (file_module{.range = {3, 1}, .file_frame = 3}));
}

@end

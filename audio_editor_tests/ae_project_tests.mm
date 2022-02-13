//
//  ae_project_tests.mm
//

#import <XCTest/XCTest.h>
#import <audio_editor_core/audio_editor_core_umbrella.h>
#import <cpp_utils/yas_thread.h>

using namespace yas;
using namespace yas::ae;

namespace yas::ae::test_utils {
struct project_url_stub final : project_url_for_project {
    url root_directory_value{url::file_url("/test/root")};
    url editing_file_value{url::file_url("/test/root/editing.caf")};
    url playing_directory_value{url::file_url("/test/root/playing")};
    url db_file_value{url::file_url("/test/root/db.sqlite")};

    url const &root_directory() const override {
        return this->root_directory_value;
    }

    url editing_file() const override {
        return this->editing_file_value;
    }

    url playing_directory() const override {
        return this->playing_directory_value;
    }

    url db_file() const override {
        return this->db_file_value;
    }

    static std::shared_ptr<project_url_stub> make_shared() {
        return std::shared_ptr<project_url_stub>(new project_url_stub{});
    }
};

struct file_importer_stub final : file_importer_for_project {
    std::function<bool(url const &, url const &)> import_handler{[](url const &, url const &) { return false; }};
    std::function<void(std::string const &)> cancel_handler{[](std::string const &) {}};

    void import(file_importing_context &&context) override {
        bool result = this->import_handler(context.src_url, context.dst_url);

        thread::perform_async_on_main([result, completion = std::move(context.completion)] { completion(result); });
    }

    void cancel(std::string const &cancel_id) override {
        this->cancel_handler(cancel_id);
    }
};

struct file_loader_stub final : file_loader_for_project {
    std::optional<file_info> file_info_value = std::nullopt;

    std::optional<file_info> load_file_info(url const &) const override {
        return this->file_info_value;
    }
};

struct player_stub final : player_for_project {
    frame_index_t current_frame() const override {
        return 0;
    }
};

struct project_editor_stub final : project_editor_for_project {
    ae::file_info file_info_value;

    ae::file_info const &file_info() const override {
        return file_info_value;
    }
    std::shared_ptr<timing_for_project_editor> const &timing() const override {
        static std::shared_ptr<timing_for_project_editor> const _null = nullptr;
        return _null;
    }

    frame_index_t current_frame() const override {
        return 0;
    }
    bool is_playing() const override {
        return false;
    }
    bool is_scrolling() const override {
        return false;
    }

    bool can_nudge() const override {
        return false;
    }
    std::size_t nudging_unit_index() const override {
        return 0;
    }
    ae::timing_fraction_kind timing_fraction_kind() const override {
        return timing_fraction_kind::sample;
    }
    bool can_jump_to_previous_edge() const override {
        return false;
    }
    bool can_jump_to_next_edge() const override {
        return false;
    }
    bool can_jump_to_beginnig() const override {
        return false;
    }
    bool can_jump_to_end() const override {
        return false;
    }
    bool can_split() const override {
        return false;
    }
    bool can_erase() const override {
        return false;
    }
    bool can_insert_marker() const override {
        return false;
    }
    bool can_return_to_zero() const override {
        return false;
    }
    bool can_go_to_marker(std::size_t const) const override {
        return false;
    }
    bool can_undo() const override {
        return false;
    }
    bool can_redo() const override {
        return false;
    }
    bool can_select_file_for_export() const override {
        return false;
    }
    bool can_cut() const override {
        return false;
    }
    bool can_copy() const override {
        return false;
    }
    bool can_paste() const override {
        return false;
    }
    bool can_begin_time_editing() const override {
        return false;
    }
    bool can_end_time_editing() const override {
        return false;
    }

    std::map<frame_index_t, marker> const &markers() const override {
        return ae::empty_markers;
    }
    file_track_module_map_t const &modules() const override {
        return ae::empty_file_track_modules;
    }

    ae::edge const &edge() const override {
        return ae::edge::zero();
    }

    observing::syncable observe_is_playing(std::function<void(bool const &)> &&) override {
        return observing::syncable{};
    }
    observing::syncable observe_file_track_event(std::function<void(file_track_event const &)> &&) override {
        return observing::syncable{};
    }
    observing::syncable observe_marker_pool_event(std::function<void(marker_pool_event const &)> &&) override {
        return observing::syncable{};
    }
    observing::syncable observe_edge_editor_event(std::function<void(edge_editor_event const &)> &&) override {
        return observing::syncable{};
    }
    observing::syncable observe_nudging_unit_index(std::function<void(std::size_t const &)> &&) override {
        return observing::syncable{};
    }
    observing::syncable observe_timing_fraction(std::function<void(ae::timing_fraction_kind const &)> &&) override {
        return observing::syncable{};
    }
    observing::syncable observe_time_editor_for_time_presenter(
        std::function<void(std::shared_ptr<time_editor_for_time_presenter> const &)> &&) override {
        return observing::syncable{};
    }
};

struct project_editor_maker_stub final : project_editor_maker_for_project {
    [[nodiscard]] std::shared_ptr<project_editor_for_project> make(url const &, url const &,
                                                                   file_info const &info) const override {
        auto editor = std::make_shared<project_editor_stub>();
        editor->file_info_value = info;
        return editor;
    }
};

struct zooming_stub final : zooming_for_project {
    void begin() override {
    }
    void set_magnification(double const) override {
    }
    void end() override {
    }

    double scale() const override {
        return 0.0;
    }

    observing::syncable observe_scale(std::function<void(double const &)> &&) override {
        return observing::syncable{};
    }
};

struct scrolling_stub final : scrolling_for_project {
    void begin() override {
    }
    void set_delta_time(double const) override {
    }
    void end() override {
    }
    bool is_began() const override {
        return false;
    }
    void set_is_enabled(bool const) override {
    }

    observing::endable observe(std::function<void(scrolling_event const &)> &&) override {
        return observing::endable{};
    }
};
}

@interface ae_project_tests : XCTestCase

@end

@implementation ae_project_tests

- (void)test_make_shared {
    auto const file_url = url::file_url("/test/path/file.wav");
    auto const project_url = std::make_shared<test_utils::project_url_stub>();
    auto const file_importer = std::make_shared<test_utils::file_importer_stub>();
    auto const file_loader = std::make_shared<test_utils::file_loader_stub>();
    auto const player = std::make_shared<test_utils::player_stub>();
    auto const editor_maker = std::make_shared<test_utils::project_editor_maker_stub>();
    auto const horizontal_zooming = std::make_shared<test_utils::zooming_stub>();
    auto const vertical_zooming = std::make_shared<test_utils::zooming_stub>();
    auto const scrolling = std::make_shared<test_utils::scrolling_stub>();
    auto const action_router = ae::action_router::make_shared();
    auto const action_controller = ae::action_controller::make_shared(action_router);
    auto const dialog_presenter = ae::dialog_presenter::make_shared();

    auto project = project::make_shared("test_uuid", file_url, project_url, file_importer, file_loader, player,
                                        editor_maker, horizontal_zooming, vertical_zooming, scrolling,
                                        action_controller, dialog_presenter, action_router);

    XCTAssertTrue(project != nullptr);
    XCTAssertEqual(project->file_url(), file_url);
}

- (void)test_loading {
    auto const src_file_url = url::file_url("/test/path/src_file.wav");
    auto const project_url = std::make_shared<test_utils::project_url_stub>();
    auto const file_importer = std::make_shared<test_utils::file_importer_stub>();
    auto const file_loader = std::make_shared<test_utils::file_loader_stub>();
    auto const player = std::make_shared<test_utils::player_stub>();
    auto const editor_maker = std::make_shared<test_utils::project_editor_maker_stub>();
    auto const horizontal_zooming = std::make_shared<test_utils::zooming_stub>();
    auto const vertical_zooming = std::make_shared<test_utils::zooming_stub>();
    auto const scrolling = std::make_shared<test_utils::scrolling_stub>();
    auto const action_router = ae::action_router::make_shared();
    auto const action_controller = ae::action_controller::make_shared(action_router);
    auto const dialog_presenter = ae::dialog_presenter::make_shared();

    struct called_values {
        url src_url;
        url dst_url;
    };

    std::optional<called_values> called;

    file_importer->import_handler = [&called](url const &src_url, url const &dst_url) {
        called = called_values{.src_url = src_url, .dst_url = dst_url};
        return false;
    };

    auto const project = project::make_shared("TEST_PROJECT_ID", src_file_url, project_url, file_importer, file_loader,
                                              player, editor_maker, horizontal_zooming, vertical_zooming, scrolling,
                                              action_controller, dialog_presenter, action_router);

    XCTAssertTrue(called.has_value());
    XCTAssertEqual(called->src_url.path(), "/test/path/src_file.wav");
    XCTAssertEqual(called->dst_url.path(), "/test/root/editing.caf");

    XCTAssertEqual(project->state(), project_state::loading);
}

- (void)test_import_success {
    auto const src_file_url = url::file_url("/test/path/src_file.wav");
    auto const project_url = std::make_shared<test_utils::project_url_stub>();
    auto const file_importer = std::make_shared<test_utils::file_importer_stub>();
    auto const file_loader = std::make_shared<test_utils::file_loader_stub>();
    auto const player = std::make_shared<test_utils::player_stub>();
    auto const editor_maker = std::make_shared<test_utils::project_editor_maker_stub>();
    auto const horizontal_zooming = std::make_shared<test_utils::zooming_stub>();
    auto const vertical_zooming = std::make_shared<test_utils::zooming_stub>();
    auto const scrolling = std::make_shared<test_utils::scrolling_stub>();
    auto const action_router = ae::action_router::make_shared();
    auto const action_controller = ae::action_controller::make_shared(action_router);
    auto const dialog_presenter = ae::dialog_presenter::make_shared();

    file_importer->import_handler = [](url const &, url const &) { return true; };
    file_loader->file_info_value = {.sample_rate = 48000, .channel_count = 1, .length = 2};

    auto const project = project::make_shared("TEST_PROJECT_ID", src_file_url, project_url, file_importer, file_loader,
                                              player, editor_maker, horizontal_zooming, vertical_zooming, scrolling,
                                              action_controller, dialog_presenter, action_router);

    std::vector<project_state> called;

    auto expectation = [self expectationWithDescription:@""];

    auto canceller = project
                         ->observe_state([&called, &expectation](auto const &state) {
                             called.emplace_back(state);

                             switch (state) {
                                 case project_state::launching:
                                 case project_state::loading:
                                 case project_state::closing:
                                     break;
                                 case project_state::editing:
                                     [expectation fulfill];
                                     break;
                                 case project_state::failure:
                                     XCTFail();
                                     break;
                             }
                         })
                         .sync();

    XCTAssertEqual(project->state(), project_state::loading);
    XCTAssertEqual(called.size(), 1);
    XCTAssertEqual(called.at(0), project_state::loading);

    [self waitForExpectations:@[expectation] timeout:10.0];

    XCTAssertEqual(project->state(), project_state::editing);
    XCTAssertEqual(called.size(), 2);
    XCTAssertEqual(called.at(1), project_state::editing);

    std::shared_ptr<project_editor_for_track_presenter> const editor = project->editor();
    XCTAssertEqual(editor->file_info(), (file_info{.sample_rate = 48000, .channel_count = 1, .length = 2}));
}

- (void)test_import_failure {
    auto const src_file_url = url::file_url("/test/path/src_file.wav");
    auto const project_url = std::make_shared<test_utils::project_url_stub>();
    auto const file_importer = std::make_shared<test_utils::file_importer_stub>();
    auto const file_loader = std::make_shared<test_utils::file_loader_stub>();
    auto const player = std::make_shared<test_utils::player_stub>();
    auto const editor_maker = std::make_shared<test_utils::project_editor_maker_stub>();
    auto const horizontal_zooming = std::make_shared<test_utils::zooming_stub>();
    auto const vertical_zooming = std::make_shared<test_utils::zooming_stub>();
    auto const scrolling = std::make_shared<test_utils::scrolling_stub>();
    auto const action_router = ae::action_router::make_shared();
    auto const action_controller = ae::action_controller::make_shared(action_router);
    auto const dialog_presenter = ae::dialog_presenter::make_shared();

    file_importer->import_handler = [](url const &, url const &) { return false; };
    file_loader->file_info_value = {.sample_rate = 96000, .channel_count = 2, .length = 3};

    auto const project = project::make_shared("TEST_PROJECT_ID", src_file_url, project_url, file_importer, file_loader,
                                              player, editor_maker, horizontal_zooming, vertical_zooming, scrolling,
                                              action_controller, dialog_presenter, action_router);

    std::vector<project_state> called;

    auto expectation = [self expectationWithDescription:@""];

    auto canceller = project
                         ->observe_state([&called, &expectation](auto const &state) {
                             called.emplace_back(state);

                             switch (state) {
                                 case project_state::launching:
                                 case project_state::loading:
                                 case project_state::closing:
                                     break;
                                 case project_state::editing:
                                     XCTFail();
                                     break;
                                 case project_state::failure:
                                     [expectation fulfill];
                                     break;
                             }
                         })
                         .sync();

    XCTAssertEqual(project->state(), project_state::loading);
    XCTAssertEqual(called.size(), 1);
    XCTAssertEqual(called.at(0), project_state::loading);
    XCTAssertFalse(project->editor());

    [self waitForExpectations:@[expectation] timeout:10.0];

    XCTAssertEqual(project->state(), project_state::failure);
    XCTAssertEqual(called.size(), 2);
    XCTAssertEqual(called.at(1), project_state::failure);
    XCTAssertFalse(project->editor());
}

@end

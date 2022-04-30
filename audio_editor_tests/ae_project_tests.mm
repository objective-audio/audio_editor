//
//  ae_project_tests.mm
//

#import <XCTest/XCTest.h>
#include <audio_editor_core/ae_project_status.h>
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

struct responder_stack_stub final : responder_stack_for_project {
    void push_responder(identifier const &, std::shared_ptr<ae::responder> const &) override {
    }
};

struct project_editor_level_pool_stub final : project_editor_level_pool_for_project {
    std::optional<file_info> file_info_value{std::nullopt};

    void add_level(file_info const &file_info) override {
        this->file_info_value = file_info;
    }

    void remove_level() override {
        this->file_info_value = std::nullopt;
    }

    std::shared_ptr<project_editor_level> const &level() const override {
        static std::shared_ptr<project_editor_level> const _null_level;
        return _null_level;
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
    auto const responder_stack = std::make_shared<test_utils::responder_stack_stub>();
    auto const editor_level_pool = std::make_shared<test_utils::project_editor_level_pool_stub>();
    auto const status = ae::project_status::make_shared();

    auto project = project::make_shared("test_uuid", file_url, project_url, file_importer, file_loader, responder_stack,
                                        editor_level_pool, status);

    XCTAssertTrue(project != nullptr);
}

- (void)test_loading {
    auto const src_file_url = url::file_url("/test/path/src_file.wav");
    auto const project_url = std::make_shared<test_utils::project_url_stub>();
    auto const file_importer = std::make_shared<test_utils::file_importer_stub>();
    auto const file_loader = std::make_shared<test_utils::file_loader_stub>();
    auto const responder_stack = std::make_shared<test_utils::responder_stack_stub>();
    auto const editor_level_pool = std::make_shared<test_utils::project_editor_level_pool_stub>();
    auto const status = ae::project_status::make_shared();

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
                                              responder_stack, editor_level_pool, status);
    project->setup();

    XCTAssertTrue(called.has_value());
    XCTAssertEqual(called->src_url.path(), "/test/path/src_file.wav");
    XCTAssertEqual(called->dst_url.path(), "/test/root/editing.caf");

    XCTAssertEqual(status->state(), project_state::loading);
}

- (void)test_import_success {
    auto const src_file_url = url::file_url("/test/path/src_file.wav");
    auto const project_url = std::make_shared<test_utils::project_url_stub>();
    auto const file_importer = std::make_shared<test_utils::file_importer_stub>();
    auto const file_loader = std::make_shared<test_utils::file_loader_stub>();
    auto const responder_stack = std::make_shared<test_utils::responder_stack_stub>();
    auto const editor_level_pool = std::make_shared<test_utils::project_editor_level_pool_stub>();
    auto const status = ae::project_status::make_shared();

    file_importer->import_handler = [](url const &, url const &) { return true; };
    file_loader->file_info_value = {.sample_rate = 48000, .channel_count = 1, .length = 2};

    auto const project = project::make_shared("TEST_PROJECT_ID", src_file_url, project_url, file_importer, file_loader,
                                              responder_stack, editor_level_pool, status);
    project->setup();

    std::vector<project_state> called;

    auto expectation = [self expectationWithDescription:@""];

    auto canceller = status
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

    XCTAssertEqual(status->state(), project_state::loading);
    XCTAssertEqual(called.size(), 1);
    XCTAssertEqual(called.at(0), project_state::loading);

    [self waitForExpectations:@[expectation] timeout:10.0];

    XCTAssertEqual(status->state(), project_state::editing);
    XCTAssertEqual(called.size(), 2);
    XCTAssertEqual(called.at(1), project_state::editing);

    XCTAssertEqual(editor_level_pool->file_info_value,
                   (file_info{.sample_rate = 48000, .channel_count = 1, .length = 2}));
}

- (void)test_import_failure {
    auto const src_file_url = url::file_url("/test/path/src_file.wav");
    auto const project_url = std::make_shared<test_utils::project_url_stub>();
    auto const file_importer = std::make_shared<test_utils::file_importer_stub>();
    auto const file_loader = std::make_shared<test_utils::file_loader_stub>();
    auto const responder_stack = std::make_shared<test_utils::responder_stack_stub>();
    auto const editor_level_pool = std::make_shared<test_utils::project_editor_level_pool_stub>();
    auto const status = ae::project_status::make_shared();

    file_importer->import_handler = [](url const &, url const &) { return false; };
    file_loader->file_info_value = {.sample_rate = 96000, .channel_count = 2, .length = 3};

    auto const project = project::make_shared("TEST_PROJECT_ID", src_file_url, project_url, file_importer, file_loader,
                                              responder_stack, editor_level_pool, status);
    project->setup();

    std::vector<project_state> called;

    auto expectation = [self expectationWithDescription:@""];

    auto canceller = status
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

    XCTAssertEqual(status->state(), project_state::loading);
    XCTAssertEqual(called.size(), 1);
    XCTAssertEqual(called.at(0), project_state::loading);
    XCTAssertFalse(editor_level_pool->file_info_value.has_value());

    [self waitForExpectations:@[expectation] timeout:10.0];

    XCTAssertEqual(status->state(), project_state::failure);
    XCTAssertEqual(called.size(), 2);
    XCTAssertEqual(called.at(1), project_state::failure);
    XCTAssertFalse(editor_level_pool->file_info_value.has_value());
}

@end

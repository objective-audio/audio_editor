//
//  ae_project_tests.mm
//

#import <XCTest/XCTest.h>
#import <audio_editor_core/audio_editor_core_umbrella.h>
#import <cpp_utils/yas_thread.h>

using namespace yas;
using namespace yas::ae;

namespace yas::ae::test_utils {
struct project_url_stub : project_url_interface {
    url root_directory_value{url::file_url("/test/root")};
    url editing_file_value{url::file_url("/test/root/editing.caf")};

    url const &root_directory() const override {
        return this->root_directory_value;
    }

    url editing_file() const override {
        return this->editing_file_value;
    }

    static std::shared_ptr<project_url_stub> make_shared() {
        return std::shared_ptr<project_url_stub>(new project_url_stub{});
    }
};

struct file_importer_stub : project_file_importer_interface {
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
}

@interface ae_project_tests : XCTestCase

@end

@implementation ae_project_tests

- (void)test_make_shared {
    auto const file_url = url::file_url("/test/path/file.wav");
    auto project = project::make_shared("test_uuid", file_url);

    XCTAssertTrue(project != nullptr);
    XCTAssertEqual(project->file_url(), file_url);
}

- (void)test_loading {
    auto const src_file_url = url::file_url("/test/path/src_file.wav");
    auto const project_url = std::make_shared<test_utils::project_url_stub>();
    auto const file_importer = std::make_shared<test_utils::file_importer_stub>();

    struct called_values {
        url src_url;
        url dst_url;
    };

    std::optional<called_values> called;

    file_importer->import_handler = [&called](url const &src_url, url const &dst_url) {
        called = called_values{.src_url = src_url, .dst_url = dst_url};
        return false;
    };

    auto const project = project::make_shared("TEST_PROJECT_ID", src_file_url, project_url, file_importer);

    XCTAssertTrue(called.has_value());
    XCTAssertEqual(called->src_url.path(), "/test/path/src_file.wav");
    XCTAssertEqual(called->dst_url.path(), "/test/root/editing.caf");

    XCTAssertEqual(project->state(), project_state::loading);
}

- (void)test_import_success {
    auto const src_file_url = url::file_url("/test/path/src_file.wav");
    auto const project_url = std::make_shared<test_utils::project_url_stub>();
    auto const file_importer = std::make_shared<test_utils::file_importer_stub>();

    file_importer->import_handler = [](url const &, url const &) { return true; };

    auto const project = project::make_shared("TEST_PROJECT_ID", src_file_url, project_url, file_importer);

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
                                 case project_state::failure:
                                     [expectation fulfill];
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
}

- (void)test_import_failure {
    auto const src_file_url = url::file_url("/test/path/src_file.wav");
    auto const project_url = std::make_shared<test_utils::project_url_stub>();
    auto const file_importer = std::make_shared<test_utils::file_importer_stub>();

    file_importer->import_handler = [](url const &, url const &) { return false; };

    auto const project = project::make_shared("TEST_PROJECT_ID", src_file_url, project_url, file_importer);

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
                                 case project_state::failure:
                                     [expectation fulfill];
                                     break;
                             }
                         })
                         .sync();

    XCTAssertEqual(project->state(), project_state::loading);
    XCTAssertEqual(called.size(), 1);
    XCTAssertEqual(called.at(0), project_state::loading);

    [self waitForExpectations:@[expectation] timeout:10.0];

    XCTAssertEqual(project->state(), project_state::failure);
    XCTAssertEqual(called.size(), 2);
    XCTAssertEqual(called.at(1), project_state::failure);
}

@end

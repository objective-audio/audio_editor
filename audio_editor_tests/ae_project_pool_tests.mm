//
//  ae_project_pool_tests.mm
//

#import <XCTest/XCTest.h>
#import <audio_editor_core/audio_editor_core_umbrella.h>

using namespace yas;
using namespace yas::ae;

namespace yas::ae::test_utils {
struct project_stub : project_for_project_pool {
    url file_url_value{""};
    std::string identifier_value;
    observing::notifier_ptr<project_event> const event_notifier = observing::notifier<project_event>::make_shared();

    std::string const &identifier() const override {
        return this->identifier_value;
    }

    std::shared_ptr<project_editor_for_project> const &editor() const override {
        static std::shared_ptr<project_editor_for_project> const _value = nullptr;
        return _value;
    }

    std::shared_ptr<zooming_for_project> const &zooming() const override {
        static std::shared_ptr<zooming_for_project> const _value = nullptr;
        return _value;
    }

    std::shared_ptr<scrolling_for_project> const &scrolling() const override {
        static std::shared_ptr<scrolling_for_project> const _value = nullptr;
        return _value;
    }

    [[nodiscard]] url const &file_url() const override {
        return this->file_url_value;
    }

    [[nodiscard]] bool can_close() const override {
        return false;
    }

    void request_close() override {
        event_notifier->notify(project_event::should_close);
    }

    project_state const &state() const override {
        static project_state constexpr _value = project_state::launching;
        return _value;
    }

    observing::endable observe_event(std::function<void(project_event const &)> &&handler) override {
        return event_notifier->observe(std::move(handler));
    }

    observing::syncable observe_state(std::function<void(project_state const &)> &&) override {
        return observing::syncable{};
    }
};

struct project_maker_stub : project_maker_for_project_pool {
    std::function<std::string(void)> generate_handler = [] { return ""; };

    std::shared_ptr<project_for_project_pool> make(url const &file_url) override {
        auto project = std::make_shared<project_stub>();
        project->identifier_value = generate_handler();
        project->file_url_value = file_url;
        return project;
    }
};
};

@interface ae_project_pool_tests : XCTestCase

@end

@implementation ae_project_pool_tests

- (void)test_add_and_return_project {
    std::string project_id;

    auto const maker = std::make_shared<test_utils::project_maker_stub>();
    maker->generate_handler = [&project_id] { return project_id; };

    auto const pool = project_pool::make_shared(maker);

    std::vector<project_pool_event> called;

    auto canceller = pool->observe_event([&called](auto const &event) { called.emplace_back(event); }).sync();

    XCTAssertEqual(called.size(), 0);

    project_id = "PROJECT_ID_0";
    auto const file_url_0 = url::file_url("/test/path/file0.wav");
    auto const project0 = pool->add_and_return_project(file_url_0);

    XCTAssertEqual(project0->identifier(), "PROJECT_ID_0");
    XCTAssertEqual(project0->file_url(), file_url_0);
    XCTAssertEqual(called.size(), 1);
    XCTAssertEqual(called.at(0).type, project_pool_event_type::inserted);
    XCTAssertEqual(called.at(0).project_id, "PROJECT_ID_0");

    project_id = "PROJECT_ID_1";
    auto const file_url_1 = url::file_url("/test/path/file1.aif");
    auto const project1 = pool->add_and_return_project(file_url_1);

    XCTAssertEqual(project1->identifier(), "PROJECT_ID_1");
    XCTAssertEqual(project1->file_url(), file_url_1);
    XCTAssertEqual(called.size(), 2);
    XCTAssertEqual(called.at(1).type, project_pool_event_type::inserted);
    XCTAssertEqual(called.at(1).project_id, "PROJECT_ID_1");
}

- (void)test_remove_project {
    std::string project_id;

    auto const maker = std::make_shared<test_utils::project_maker_stub>();
    maker->generate_handler = [&project_id] { return project_id; };

    auto const pool = project_pool::make_shared(maker);

    project_id = "PROJECT_ID_0";
    auto const file_url_0 = url::file_url("/test/path/file0.wav");
    auto const project0 = pool->add_and_return_project(file_url_0);

    project_id = "PROJECT_ID_1";
    auto const file_url_1 = url::file_url("/test/path/file1.aif");
    auto const project1 = pool->add_and_return_project(file_url_1);

    std::vector<project_pool_event> called;

    auto canceller = pool->observe_event([&called](auto const &event) { called.emplace_back(event); }).sync();

    XCTAssertEqual(called.size(), 2);

    project0->request_close();
    XCTAssertEqual(pool->size(), 1);
    XCTAssertEqual(called.size(), 3);
    XCTAssertEqual(called.at(2).type, project_pool_event_type::erased);
    XCTAssertEqual(called.at(2).project_id, "PROJECT_ID_0");

    project1->request_close();
    XCTAssertEqual(pool->size(), 0);
    XCTAssertEqual(called.size(), 4);
    XCTAssertEqual(called.at(3).type, project_pool_event_type::erased);
    XCTAssertEqual(called.at(3).project_id, "PROJECT_ID_1");
}

@end

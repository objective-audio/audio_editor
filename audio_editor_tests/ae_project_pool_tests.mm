//
//  ae_project_pool_tests.mm
//

#import <XCTest/XCTest.h>
#import <audio_editor_core/audio_editor_core_umbrella.h>

using namespace yas;
using namespace yas::ae;

namespace yas::ae::test_utils {
struct uuid_generator_stub : uuid_generator_for_project_pool {
    std::function<std::string(void)> generate_handler = [] { return ""; };

    std::string generate() const override {
        return this->generate_handler();
    }

    static std::shared_ptr<uuid_generator_stub> make_shared() {
        return std::make_shared<uuid_generator_stub>();
    }
};
};

@interface ae_project_pool_tests : XCTestCase

@end

@implementation ae_project_pool_tests

- (void)test_add_and_return_project {
    auto const generator = test_utils::uuid_generator_stub::make_shared();
    std::string project_id;
    generator->generate_handler = [&project_id] { return project_id; };

    auto const pool = project_pool::make_shared(generator);

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
    auto const generator = test_utils::uuid_generator_stub::make_shared();
    std::string project_id;
    generator->generate_handler = [&project_id] { return project_id; };

    auto const pool = project_pool::make_shared(generator);

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

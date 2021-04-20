//
//  ae_project_pool_tests.mm
//

#import <XCTest/XCTest.h>
#import <audio_editor_core/audio_editor_core_umbrella.h>

using namespace yas;
using namespace yas::ae;

@interface ae_project_pool_tests : XCTestCase

@end

@implementation ae_project_pool_tests

- (void)test_add_project {
    auto const pool = project_pool::make_shared();

    std::vector<project_pool_event> called;

    auto canceller = pool->observe_project([&called](auto const &event) { called.emplace_back(event); }).sync();

    XCTAssertEqual(called.size(), 0);

    auto const file_url_0 = url::file_url("/test/path/file0.wav");
    auto const project0 = pool->add_project(file_url_0);

    XCTAssertEqual(project0->file_url(), file_url_0);
    XCTAssertEqual(called.size(), 1);
    XCTAssertEqual(called.at(0).type, project_pool_event_type::inserted);
    XCTAssertEqual(called.at(0).project_id, project0->identifier());

    auto const file_url_1 = url::file_url("/test/path/file1.aif");
    auto const project1 = pool->add_project(file_url_1);

    XCTAssertEqual(project1->file_url(), file_url_1);
    XCTAssertEqual(called.size(), 2);
    XCTAssertEqual(called.at(1).type, project_pool_event_type::inserted);
    XCTAssertEqual(called.at(1).project_id, project1->identifier());
}

- (void)test_remove_project {
    auto const pool = project_pool::make_shared();

    auto const file_url_0 = url::file_url("/test/path/file0.wav");
    auto const project0 = pool->add_project(file_url_0);

    auto const file_url_1 = url::file_url("/test/path/file1.aif");
    auto const project1 = pool->add_project(file_url_1);

    std::vector<project_pool_event> called;

    auto canceller = pool->observe_project([&called](auto const &event) { called.emplace_back(event); }).sync();

    XCTAssertEqual(called.size(), 2);

    project0->request_close();
    XCTAssertEqual(pool->projects().size(), 1);
    XCTAssertEqual(called.size(), 3);
    XCTAssertEqual(called.at(2).type, project_pool_event_type::erased);
    XCTAssertEqual(called.at(2).project_id, project0->identifier());

    project1->request_close();
    XCTAssertEqual(pool->projects().size(), 0);
    XCTAssertEqual(called.size(), 4);
    XCTAssertEqual(called.at(3).type, project_pool_event_type::erased);
    XCTAssertEqual(called.at(3).project_id, project1->identifier());
}

@end

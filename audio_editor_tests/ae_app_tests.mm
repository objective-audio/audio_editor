//
//  ae_app_tests.mm
//

#import <XCTest/XCTest.h>
#import <audio_editor_core/audio_editor_core_umbrella.h>

using namespace yas;
using namespace yas::ae;

@interface ae_app_tests : XCTestCase

@end

@implementation ae_app_tests

- (void)test_make_shared {
    auto const app = app::make_shared();

    XCTAssertTrue(app != nullptr);
    XCTAssertEqual(app->projects().size(), 0);
}

- (void)test_add_project {
    auto const app = app::make_shared();

    std::vector<app::projects_t::event> called;

    auto canceller = app->observe_projects([&called](auto const &event) { called.emplace_back(event); }).sync();

    XCTAssertEqual(called.size(), 1);
    XCTAssertEqual(called.at(0).type, observing::map::event_type::any);
    XCTAssertEqual(called.at(0).elements.size(), 0);
    XCTAssertEqual(app->projects().size(), 0);

    auto const file_url_0 = url::file_url("/test/path/file0.wav");
    app->add_project(file_url_0);

    XCTAssertEqual(called.size(), 2);
    XCTAssertEqual(called.at(1).type, observing::map::event_type::inserted);
    XCTAssertEqual(called.at(1).inserted->get()->file_url(), file_url_0);
    XCTAssertEqual(app->projects().size(), 1);

    auto const file_url_1 = url::file_url("/test/path/file1.aif");
    app->add_project(file_url_1);

    XCTAssertEqual(called.size(), 3);
    XCTAssertEqual(called.at(2).type, observing::map::event_type::inserted);
    XCTAssertEqual(called.at(2).inserted->get()->file_url(), file_url_1);
    XCTAssertEqual(app->projects().size(), 2);
}

- (void)test_remove_project {
    auto const app = app::make_shared();

    auto const file_url_0 = url::file_url("/test/path/file0.wav");
    app->add_project(file_url_0);

    auto const file_url_1 = url::file_url("/test/path/file1.aif");
    app->add_project(file_url_1);

    auto const pair0 =
        yas::first(app->projects(), [&file_url_0](auto const &pair) { return pair.second->file_url() == file_url_0; });
    XCTAssertTrue(pair0.has_value());
    pair0.value().second->request_close();
    XCTAssertEqual(app->projects().size(), 1);

    auto const pair1 =
        yas::first(app->projects(), [&file_url_1](auto const &pair) { return pair.second->file_url() == file_url_1; });
    XCTAssertTrue(pair1.has_value());
    pair1.value().second->request_close();
    XCTAssertEqual(app->projects().size(), 0);
}

@end

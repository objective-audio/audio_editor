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
    XCTAssertEqual(called.at(0).type, observing::vector::event_type::any);
    XCTAssertEqual(called.at(0).elements.size(), 0);
    XCTAssertEqual(app->projects().size(), 0);

    auto const file_url_0 = url::file_url("/test/path/file0.wav");
    app->add_project(file_url_0);

    XCTAssertEqual(called.size(), 2);
    XCTAssertEqual(called.at(1).type, observing::vector::event_type::inserted);
    XCTAssertEqual(called.at(1).index, 0);
    XCTAssertEqual(called.at(1).element->get()->file_url(), file_url_0);
    XCTAssertEqual(app->projects().size(), 1);

    auto const file_url_1 = url::file_url("/test/path/file1.aif");
    app->add_project(file_url_1);

    XCTAssertEqual(called.size(), 3);
    XCTAssertEqual(called.at(2).type, observing::vector::event_type::inserted);
    XCTAssertEqual(called.at(2).index, 1);
    XCTAssertEqual(called.at(2).element->get()->file_url(), file_url_1);
    XCTAssertEqual(app->projects().size(), 2);
}

- (void)test_remove_project {
    auto const app = app::make_shared();

    auto const file_url_0 = url::file_url("/test/path/file0.wav");
    app->add_project(file_url_0);

    auto const file_url_1 = url::file_url("/test/path/file1.aif");
    app->add_project(file_url_1);

    app->projects().at(0)->request_close();
    XCTAssertEqual(app->projects().size(), 1);

    //    XCTAssertTrue(app->projects().at(0)->requestClose());
    //    XCTAssertEqual(app->projects().size(), 0);
}

@end

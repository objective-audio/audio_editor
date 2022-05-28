//
//  ae_system_url_tests.mm
//

#import <XCTest/XCTest.h>
#import <audio_editor_core/ae_system_url.h>

using namespace yas;
using namespace yas::ae;

@interface ae_system_url_tests : XCTestCase

@end

@implementation ae_system_url_tests

- (void)test_document_directory {
    auto const system_url = system_url::make_shared();

    XCTAssertEqual(system_url->document_directory().last_path_component(), "Documents");
}

- (void)test_app_directory {
    auto const system_url = system_url::make_shared();
    auto const app_url = system_url->app_directory();

    XCTAssertEqual(app_url.last_path_component(), "audio_editor_app");

    auto document_url = app_url.deleting_last_path_component();

    XCTAssertEqual(document_url.last_path_component(), "Documents");
}

- (void)test_project_directory {
    auto const system_url = system_url::make_shared();
    auto const project_url = system_url->project_directory({"TEST_PROJECT_ID"});

    XCTAssertEqual(project_url.last_path_component(), "TEST_PROJECT_ID");

    auto const app_url = project_url.deleting_last_path_component();

    XCTAssertEqual(app_url.last_path_component(), "audio_editor_app");
}

@end

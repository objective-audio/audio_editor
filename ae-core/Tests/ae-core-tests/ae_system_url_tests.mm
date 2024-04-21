//
//  ae_system_path_tests.mm
//

#import <XCTest/XCTest.h>
#import <ae-core/app/features/ae_system_path.h>

using namespace yas;
using namespace yas::ae;

@interface ae_system_path_tests : XCTestCase

@end

@implementation ae_system_path_tests

- (void)test_document_directory {
    auto const system_path = std::make_shared<ae::system_path>();

    XCTAssertEqual(system_path->document_directory().filename(), "Documents");
}

- (void)test_app_directory {
    auto const system_path = std::make_shared<ae::system_path>();
    auto app_path = system_path->app_directory();

    XCTAssertEqual(app_path.filename(), "audio_editor_app");

    auto document_path = app_path.parent_path();

    XCTAssertEqual(document_path.filename(), "Documents");
}

@end

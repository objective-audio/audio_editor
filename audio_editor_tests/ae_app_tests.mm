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
    XCTAssertTrue(app->project_pool() != nullptr);
    XCTAssertTrue(app->system_url() != nullptr);
    XCTAssertTrue(app->file_importer() != nullptr);
}

@end

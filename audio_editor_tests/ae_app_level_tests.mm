//
//  ae_app_tests.mm
//

#import <XCTest/XCTest.h>
#import <audio_editor_core/audio_editor_core_umbrella.h>

using namespace yas;
using namespace yas::ae;

@interface ae_app_level_tests : XCTestCase

@end

@implementation ae_app_level_tests

- (void)test_make_shared {
    auto const app_level = app_level::make_shared();

    XCTAssertTrue(app_level != nullptr);

    XCTAssertTrue(app_level->worker != nullptr);
    XCTAssertTrue(app_level->system_url != nullptr);
    XCTAssertTrue(app_level->launcher != nullptr);
    XCTAssertTrue(app_level->file_importer != nullptr);
    XCTAssertTrue(app_level->file_info_loader != nullptr);
    XCTAssertTrue(app_level->color != nullptr);

    XCTAssertTrue(app_level->project_level_router != nullptr);
    XCTAssertTrue(app_level->ui_root_level_router != nullptr);
}

@end

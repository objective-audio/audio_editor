//
//  ae_app_tests.mm
//

#import <XCTest/XCTest.h>
#import <audio_editor_core/ae_app_lifetime.h>

using namespace yas;
using namespace yas::ae;

@interface ae_app_lifetime_tests : XCTestCase

@end

@implementation ae_app_lifetime_tests

- (void)test_make_shared {
    auto const lifetime = app_lifetime::make_shared();

    XCTAssertTrue(lifetime != nullptr);

    XCTAssertTrue(lifetime->worker != nullptr);
    XCTAssertTrue(lifetime->system_url != nullptr);
    XCTAssertTrue(lifetime->launcher != nullptr);
    XCTAssertTrue(lifetime->file_importer != nullptr);
    XCTAssertTrue(lifetime->file_info_loader != nullptr);
    XCTAssertTrue(lifetime->color != nullptr);

    XCTAssertTrue(lifetime->window_lifecycle != nullptr);
    XCTAssertTrue(lifetime->ui_root_lifecycle != nullptr);
}

@end

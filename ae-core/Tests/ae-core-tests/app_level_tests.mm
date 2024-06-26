//
//  app_tests.mm
//

#import <XCTest/XCTest.h>
#import <ae-core/app/app_lifetime.h>

using namespace yas;
using namespace yas::ae;

@interface app_lifetime_tests : XCTestCase

@end

@implementation app_lifetime_tests

- (void)test_make_shared {
    auto const lifetime = app_lifetime::make_shared();

    XCTAssertTrue(lifetime != nullptr);

    XCTAssertTrue(lifetime->worker != nullptr);
    XCTAssertTrue(lifetime->system_path != nullptr);
    XCTAssertTrue(lifetime->launcher != nullptr);
    XCTAssertTrue(lifetime->file_importer != nullptr);
    XCTAssertTrue(lifetime->file_info_loader != nullptr);
    XCTAssertTrue(lifetime->color != nullptr);

    XCTAssertTrue(lifetime->project_lifecycle != nullptr);
    XCTAssertTrue(lifetime->ui_resource_lifecycle != nullptr);
}

@end

//
//  ae_system_url_tests.mm
//

#import <XCTest/XCTest.h>
#import <audio_editor_core/audio_editor_core_umbrella.h>

using namespace yas;
using namespace yas::ae;

@interface ae_system_url_tests : XCTestCase

@end

@implementation ae_system_url_tests

- (void)test_document_directory {
    auto const url = system_url::make_shared();

    XCTAssertEqual(url->document_directory().last_path_component(), "Documents");
}

@end

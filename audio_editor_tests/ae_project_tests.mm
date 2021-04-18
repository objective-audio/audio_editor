//
//  ae_project_tests.mm
//

#import <XCTest/XCTest.h>
#import <audio_editor_core/audio_editor_core_umbrella.h>

using namespace yas;
using namespace yas::ae;

@interface ae_project_tests : XCTestCase

@end

@implementation ae_project_tests

- (void)test_make_shared {
    auto const file_url = url::file_url("/test/path/file.wav");
    auto project = project::make_shared(file_url);

    XCTAssertTrue(project != nullptr);
    XCTAssertEqual(project->file_url(), file_url);
}

@end

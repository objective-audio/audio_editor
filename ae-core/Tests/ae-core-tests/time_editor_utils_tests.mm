//
//  time_editor_utils_tests.mm
//

#import <XCTest/XCTest.h>
#include <ae-core/time_editor/utils/time_editor_utils.h>

using namespace yas;
using namespace yas::ae;

@interface time_editor_utils_tests : XCTestCase

@end

@implementation time_editor_utils_tests

- (void)test_to_editing_components {
    XCTAssertEqual(time_editor_utils::to_editing_components(
                       number_components{true, {{.size = 2, .value = 1}, {.size = 60, .value = 23}}}),
                   (number_components{true, {{.size = 10, .value = 1}, {.size = 100, .value = 23}}}));
}

@end

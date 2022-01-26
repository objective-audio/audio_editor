//
//  ae_time_presenter_utils_tests.mm
//

#import <XCTest/XCTest.h>
#include <audio_editor_core/ae_time_presenter_utils.h>
#include <audio_editor_core/ae_timing.h>

using namespace yas;
using namespace yas::ae;

@interface ae_time_presenter_utils_tests : XCTestCase

@end

@implementation ae_time_presenter_utils_tests

- (void)test_time_text {
    number_components const plus_components{false,
                                            {{.size = 1000, .value = 123},
                                             {.size = 60, .value = 45},
                                             {.size = 60, .value = 56},
                                             {.size = 99, .value = 78}}};

    XCTAssertEqual(time_presenter_utils::time_text(plus_components), "+78:56:45.123");

    number_components const minus_components{
        true,
        {{.size = 1000, .value = 987}, {.size = 60, .value = 54}, {.size = 60, .value = 32}, {.size = 99, .value = 1}}};

    XCTAssertEqual(time_presenter_utils::time_text(minus_components), "-01:32:54.987");
}

@end

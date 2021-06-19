//
//  ae_project_view_presenter_utils_tests.mm
//

#import <XCTest/XCTest.h>
#import <audio_editor_core/ae_project_view_presenter_utils.h>

using namespace yas;
using namespace yas::ae;

@interface ae_project_view_presenter_utils_tests : XCTestCase

@end

@implementation ae_project_view_presenter_utils_tests

- (void)test_time_text_plus {
    XCTAssertEqual(project_view_presenter_utils::time_text(0, 1000), "0:00:00.000");
    XCTAssertEqual(project_view_presenter_utils::time_text(1, 1000), "0:00:00.001");
    XCTAssertEqual(project_view_presenter_utils::time_text(999, 1000), "0:00:00.999");
    XCTAssertEqual(project_view_presenter_utils::time_text(1000, 1000), "0:00:01.000");
    XCTAssertEqual(project_view_presenter_utils::time_text(1001, 1000), "0:00:01.001");
    XCTAssertEqual(project_view_presenter_utils::time_text(59999, 1000), "0:00:59.999");
    XCTAssertEqual(project_view_presenter_utils::time_text(60000, 1000), "0:01:00.000");
    XCTAssertEqual(project_view_presenter_utils::time_text(60001, 1000), "0:01:00.001");
    XCTAssertEqual(project_view_presenter_utils::time_text(3599999, 1000), "0:59:59.999");
    XCTAssertEqual(project_view_presenter_utils::time_text(3600000, 1000), "1:00:00.000");
    XCTAssertEqual(project_view_presenter_utils::time_text(3600001, 1000), "1:00:00.001");
}

- (void)test_time_text_minus {
    XCTAssertEqual(project_view_presenter_utils::time_text(-1, 1000), "-0:00:00.001");
    XCTAssertEqual(project_view_presenter_utils::time_text(-999, 1000), "-0:00:00.999");
    XCTAssertEqual(project_view_presenter_utils::time_text(-1000, 1000), "-0:00:01.000");
    XCTAssertEqual(project_view_presenter_utils::time_text(-1001, 1000), "-0:00:01.001");
    XCTAssertEqual(project_view_presenter_utils::time_text(-59999, 1000), "-0:00:59.999");
    XCTAssertEqual(project_view_presenter_utils::time_text(-60000, 1000), "-0:01:00.000");
    XCTAssertEqual(project_view_presenter_utils::time_text(-60001, 1000), "-0:01:00.001");
    XCTAssertEqual(project_view_presenter_utils::time_text(-3599999, 1000), "-0:59:59.999");
    XCTAssertEqual(project_view_presenter_utils::time_text(-3600000, 1000), "-1:00:00.000");
    XCTAssertEqual(project_view_presenter_utils::time_text(-3600001, 1000), "-1:00:00.001");
}

- (void)test_after_point_digits {
    XCTAssertEqual(project_view_presenter_utils::after_point_digits(1), 0);
    XCTAssertEqual(project_view_presenter_utils::after_point_digits(2), 1);
    XCTAssertEqual(project_view_presenter_utils::after_point_digits(9), 1);
    XCTAssertEqual(project_view_presenter_utils::after_point_digits(10), 1);
    XCTAssertEqual(project_view_presenter_utils::after_point_digits(11), 2);
    XCTAssertEqual(project_view_presenter_utils::after_point_digits(99), 2);
    XCTAssertEqual(project_view_presenter_utils::after_point_digits(100), 2);
    XCTAssertEqual(project_view_presenter_utils::after_point_digits(101), 3);
    XCTAssertEqual(project_view_presenter_utils::after_point_digits(999), 3);
    XCTAssertEqual(project_view_presenter_utils::after_point_digits(1000), 3);
    XCTAssertEqual(project_view_presenter_utils::after_point_digits(1001), 4);
}

- (void)test_time_text_after_point_digit {
    XCTAssertEqual(project_view_presenter_utils::time_text(0, 1), "0:00:00");
    XCTAssertEqual(project_view_presenter_utils::time_text(1, 1), "0:00:01");

    XCTAssertEqual(project_view_presenter_utils::time_text(0, 2), "0:00:00.0");
    XCTAssertEqual(project_view_presenter_utils::time_text(1, 2), "0:00:00.1");
    XCTAssertEqual(project_view_presenter_utils::time_text(2, 2), "0:00:01.0");

    XCTAssertEqual(project_view_presenter_utils::time_text(0, 44100), "0:00:00.00000");
    XCTAssertEqual(project_view_presenter_utils::time_text(44099, 44100), "0:00:00.44099");
    XCTAssertEqual(project_view_presenter_utils::time_text(44100, 44100), "0:00:01.00000");
}

@end

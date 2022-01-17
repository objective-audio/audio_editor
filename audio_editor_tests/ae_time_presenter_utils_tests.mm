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

- (void)test_time_text_plus {
    auto const timing = timing::make_shared(1000);

    XCTAssertEqual(time_presenter_utils::time_text(0, timing), "+00:00:00.000");
    XCTAssertEqual(time_presenter_utils::time_text(1, timing), "+00:00:00.001");
    XCTAssertEqual(time_presenter_utils::time_text(999, timing), "+00:00:00.999");
    XCTAssertEqual(time_presenter_utils::time_text(1000, timing), "+00:00:01.000");
    XCTAssertEqual(time_presenter_utils::time_text(1001, timing), "+00:00:01.001");
    XCTAssertEqual(time_presenter_utils::time_text(59999, timing), "+00:00:59.999");
    XCTAssertEqual(time_presenter_utils::time_text(60000, timing), "+00:01:00.000");
    XCTAssertEqual(time_presenter_utils::time_text(60001, timing), "+00:01:00.001");
    XCTAssertEqual(time_presenter_utils::time_text(3599999, timing), "+00:59:59.999");
    XCTAssertEqual(time_presenter_utils::time_text(3600000, timing), "+01:00:00.000");
    XCTAssertEqual(time_presenter_utils::time_text(3600001, timing), "+01:00:00.001");
}

- (void)test_time_text_minus {
    auto const timing = timing::make_shared(1000);

    XCTAssertEqual(time_presenter_utils::time_text(-1, timing), "-00:00:00.001");
    XCTAssertEqual(time_presenter_utils::time_text(-999, timing), "-00:00:00.999");
    XCTAssertEqual(time_presenter_utils::time_text(-1000, timing), "-00:00:01.000");
    XCTAssertEqual(time_presenter_utils::time_text(-1001, timing), "-00:00:01.001");
    XCTAssertEqual(time_presenter_utils::time_text(-59999, timing), "-00:00:59.999");
    XCTAssertEqual(time_presenter_utils::time_text(-60000, timing), "-00:01:00.000");
    XCTAssertEqual(time_presenter_utils::time_text(-60001, timing), "-00:01:00.001");
    XCTAssertEqual(time_presenter_utils::time_text(-3599999, timing), "-00:59:59.999");
    XCTAssertEqual(time_presenter_utils::time_text(-3600000, timing), "-01:00:00.000");
    XCTAssertEqual(time_presenter_utils::time_text(-3600001, timing), "-01:00:00.001");
}

- (void)test_time_text_sample {
    {
        auto const timing = timing::make_shared(1);

        XCTAssertEqual(time_presenter_utils::time_text(0, timing), "+00:00:00.0");
        XCTAssertEqual(time_presenter_utils::time_text(1, timing), "+00:00:01.0");
    }

    {
        auto const timing = timing::make_shared(2);

        XCTAssertEqual(time_presenter_utils::time_text(0, timing), "+00:00:00.0");
        XCTAssertEqual(time_presenter_utils::time_text(1, timing), "+00:00:00.1");
        XCTAssertEqual(time_presenter_utils::time_text(2, timing), "+00:00:01.0");
    }

    {
        auto const timing = timing::make_shared(44100);

        XCTAssertEqual(time_presenter_utils::time_text(0, timing), "+00:00:00.00000");
        XCTAssertEqual(time_presenter_utils::time_text(44099, timing), "+00:00:00.44099");
        XCTAssertEqual(time_presenter_utils::time_text(44100, timing), "+00:00:01.00000");
    }
}

- (void)test_time_text_milisecond {
    auto const timing = timing::make_shared(48000);

    timing->set_fraction_kind(timing_fraction_kind::milisecond);

    XCTAssertEqual(time_presenter_utils::time_text(0, timing), "+00:00:00.000");
    XCTAssertEqual(time_presenter_utils::time_text(47, timing), "+00:00:00.000");
    XCTAssertEqual(time_presenter_utils::time_text(48, timing), "+00:00:00.001");
    XCTAssertEqual(time_presenter_utils::time_text(47999, timing), "+00:00:00.999");
    XCTAssertEqual(time_presenter_utils::time_text(48000, timing), "+00:00:01.000");

    XCTAssertEqual(time_presenter_utils::time_text(-47, timing), "-00:00:00.000");
    XCTAssertEqual(time_presenter_utils::time_text(-48, timing), "-00:00:00.001");
    XCTAssertEqual(time_presenter_utils::time_text(-47999, timing), "-00:00:00.999");
    XCTAssertEqual(time_presenter_utils::time_text(-48000, timing), "-00:00:01.000");
}

- (void)test_time_text_frame30 {
    auto const timing = timing::make_shared(48000);

    timing->set_fraction_kind(timing_fraction_kind::frame30);

    XCTAssertEqual(time_presenter_utils::time_text(0, timing), "+00:00:00.00");
    XCTAssertEqual(time_presenter_utils::time_text(1599, timing), "+00:00:00.00");
    XCTAssertEqual(time_presenter_utils::time_text(1600, timing), "+00:00:00.01");
    XCTAssertEqual(time_presenter_utils::time_text(47999, timing), "+00:00:00.29");
    XCTAssertEqual(time_presenter_utils::time_text(48000, timing), "+00:00:01.00");

    XCTAssertEqual(time_presenter_utils::time_text(-1599, timing), "-00:00:00.00");
    XCTAssertEqual(time_presenter_utils::time_text(-1600, timing), "-00:00:00.01");
    XCTAssertEqual(time_presenter_utils::time_text(-47999, timing), "-00:00:00.29");
    XCTAssertEqual(time_presenter_utils::time_text(-48000, timing), "-00:00:01.00");
}

@end

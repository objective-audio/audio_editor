//
//  timing_utils_tests.mm
//

#import <XCTest/XCTest.h>
#include <ae-core/project_editing/features/timing_utils.h>

using namespace yas;
using namespace yas::ae;

@interface timing_utils_tests : XCTestCase

@end

@implementation timing_utils_tests

- (void)test_to_fraction_unit {
    XCTAssertEqual(timing_utils::to_fraction_unit(timing_fraction_kind::sample, 48000), 48000);
    XCTAssertEqual(timing_utils::to_fraction_unit(timing_fraction_kind::milisecond, 48000), 1000);
    XCTAssertEqual(timing_utils::to_fraction_unit(timing_fraction_kind::frame30, 48000), 30);
}

- (void)test_to_fraction_digits {
    XCTAssertEqual(timing_utils::to_fraction_digits(timing_fraction_kind::sample, 48000), 5);
    XCTAssertEqual(timing_utils::to_fraction_digits(timing_fraction_kind::milisecond, 48000), 3);
    XCTAssertEqual(timing_utils::to_fraction_digits(timing_fraction_kind::frame30, 48000), 2);
}

- (void)test_to_fraction_value {
    XCTAssertEqual(timing_utils::to_fraction_value(0, timing_fraction_kind::sample, 48000), 0);
    XCTAssertEqual(timing_utils::to_fraction_value(1, timing_fraction_kind::sample, 48000), 1);
    XCTAssertEqual(timing_utils::to_fraction_value(47999, timing_fraction_kind::sample, 48000), 47999);
    XCTAssertEqual(timing_utils::to_fraction_value(48000, timing_fraction_kind::sample, 48000), 0);

    XCTAssertEqual(timing_utils::to_fraction_value(-1, timing_fraction_kind::sample, 48000), 1);
    XCTAssertEqual(timing_utils::to_fraction_value(-47999, timing_fraction_kind::sample, 48000), 47999);
    XCTAssertEqual(timing_utils::to_fraction_value(-48000, timing_fraction_kind::sample, 48000), 0);

    XCTAssertEqual(timing_utils::to_fraction_value(0, timing_fraction_kind::milisecond, 48000), 0);
    XCTAssertEqual(timing_utils::to_fraction_value(47, timing_fraction_kind::milisecond, 48000), 0);
    XCTAssertEqual(timing_utils::to_fraction_value(48, timing_fraction_kind::milisecond, 48000), 1);
    XCTAssertEqual(timing_utils::to_fraction_value(47999, timing_fraction_kind::milisecond, 48000), 999);
    XCTAssertEqual(timing_utils::to_fraction_value(48000, timing_fraction_kind::milisecond, 48000), 0);

    XCTAssertEqual(timing_utils::to_fraction_value(-47, timing_fraction_kind::milisecond, 48000), 0);
    XCTAssertEqual(timing_utils::to_fraction_value(-48, timing_fraction_kind::milisecond, 48000), 1);
    XCTAssertEqual(timing_utils::to_fraction_value(-47999, timing_fraction_kind::milisecond, 48000), 999);
    XCTAssertEqual(timing_utils::to_fraction_value(-48000, timing_fraction_kind::milisecond, 48000), 0);

    XCTAssertEqual(timing_utils::to_fraction_value(0, timing_fraction_kind::frame30, 48000), 0);
    XCTAssertEqual(timing_utils::to_fraction_value(1599, timing_fraction_kind::frame30, 48000), 0);
    XCTAssertEqual(timing_utils::to_fraction_value(1600, timing_fraction_kind::frame30, 48000), 1);
    XCTAssertEqual(timing_utils::to_fraction_value(47999, timing_fraction_kind::frame30, 48000), 29);
    XCTAssertEqual(timing_utils::to_fraction_value(48000, timing_fraction_kind::frame30, 48000), 0);

    XCTAssertEqual(timing_utils::to_fraction_value(-1599, timing_fraction_kind::frame30, 48000), 0);
    XCTAssertEqual(timing_utils::to_fraction_value(-1600, timing_fraction_kind::frame30, 48000), 1);
    XCTAssertEqual(timing_utils::to_fraction_value(-47999, timing_fraction_kind::frame30, 48000), 29);
    XCTAssertEqual(timing_utils::to_fraction_value(-48000, timing_fraction_kind::frame30, 48000), 0);
}

- (void)test_to_components {
    sample_rate_t const sample_rate = 48000;

    {
        frame_index_t const frame = 60 * 60 * sample_rate * 1 + 60 * sample_rate * 2 + sample_rate * 3 + 4;
        auto const components = timing_utils::to_components(frame, timing_fraction_kind::sample, sample_rate);
        XCTAssertFalse(components.is_minus());
        XCTAssertEqual(components.value(timing_unit_kind::hours), 1);
        XCTAssertEqual(components.value(timing_unit_kind::minutes), 2);
        XCTAssertEqual(components.value(timing_unit_kind::seconds), 3);
        XCTAssertEqual(components.value(timing_unit_kind::fraction), 4);
    }

    {
        frame_index_t const frame = 60 * 60 * sample_rate * 1 + 60 * sample_rate * 2 + sample_rate * 3 + 4;
        auto const components = timing_utils::to_components(-frame, timing_fraction_kind::sample, sample_rate);
        XCTAssertTrue(components.is_minus());
        XCTAssertEqual(components.value(timing_unit_kind::hours), 1);
        XCTAssertEqual(components.value(timing_unit_kind::minutes), 2);
        XCTAssertEqual(components.value(timing_unit_kind::seconds), 3);
        XCTAssertEqual(components.value(timing_unit_kind::fraction), 4);
    }

    {
        frame_index_t const frame =
            60 * 60 * sample_rate * 1 + 60 * sample_rate * 2 + sample_rate * 3 + sample_rate / 1000 * 4;
        auto const components = timing_utils::to_components(frame, timing_fraction_kind::milisecond, sample_rate);
        XCTAssertFalse(components.is_minus());
        XCTAssertEqual(components.value(timing_unit_kind::hours), 1);
        XCTAssertEqual(components.value(timing_unit_kind::minutes), 2);
        XCTAssertEqual(components.value(timing_unit_kind::seconds), 3);
        XCTAssertEqual(components.value(timing_unit_kind::fraction), 4);
    }

    {
        frame_index_t const frame =
            60 * 60 * sample_rate * 1 + 60 * sample_rate * 2 + sample_rate * 3 + sample_rate / 30 * 4;
        auto const components = timing_utils::to_components(frame, timing_fraction_kind::frame30, sample_rate);
        XCTAssertFalse(components.is_minus());
        XCTAssertEqual(components.value(timing_unit_kind::hours), 1);
        XCTAssertEqual(components.value(timing_unit_kind::minutes), 2);
        XCTAssertEqual(components.value(timing_unit_kind::seconds), 3);
        XCTAssertEqual(components.value(timing_unit_kind::fraction), 4);
    }
}

- (void)test_to_floored_components {
    sample_rate_t const sample_rate = 48000;

    {
        frame_index_t const frame = 60 * 60 * sample_rate * 1 + 60 * sample_rate * 2 + sample_rate * 3 + 4;

        {
            auto const components = timing_utils::to_floored_components(frame, timing_unit_kind::fraction,
                                                                        timing_fraction_kind::sample, sample_rate);
            XCTAssertFalse(components.is_minus());
            XCTAssertEqual(components.value(timing_unit_kind::hours), 1);
            XCTAssertEqual(components.value(timing_unit_kind::minutes), 2);
            XCTAssertEqual(components.value(timing_unit_kind::seconds), 3);
            XCTAssertEqual(components.value(timing_unit_kind::fraction), 4);
        }

        {
            auto const components = timing_utils::to_floored_components(frame, timing_unit_kind::seconds,
                                                                        timing_fraction_kind::sample, sample_rate);
            XCTAssertFalse(components.is_minus());
            XCTAssertEqual(components.value(timing_unit_kind::hours), 1);
            XCTAssertEqual(components.value(timing_unit_kind::minutes), 2);
            XCTAssertEqual(components.value(timing_unit_kind::seconds), 3);
            XCTAssertEqual(components.value(timing_unit_kind::fraction), 0);
        }

        {
            auto const components = timing_utils::to_floored_components(frame, timing_unit_kind::minutes,
                                                                        timing_fraction_kind::sample, sample_rate);
            XCTAssertFalse(components.is_minus());
            XCTAssertEqual(components.value(timing_unit_kind::hours), 1);
            XCTAssertEqual(components.value(timing_unit_kind::minutes), 2);
            XCTAssertEqual(components.value(timing_unit_kind::seconds), 0);
            XCTAssertEqual(components.value(timing_unit_kind::fraction), 0);
        }

        {
            auto const components = timing_utils::to_floored_components(frame, timing_unit_kind::hours,
                                                                        timing_fraction_kind::sample, sample_rate);
            XCTAssertFalse(components.is_minus());
            XCTAssertEqual(components.value(timing_unit_kind::hours), 1);
            XCTAssertEqual(components.value(timing_unit_kind::minutes), 0);
            XCTAssertEqual(components.value(timing_unit_kind::seconds), 0);
            XCTAssertEqual(components.value(timing_unit_kind::fraction), 0);
        }
    }

    {
        frame_index_t const frame = 60 * 60 * sample_rate * 1 + 60 * sample_rate * 2 + sample_rate * 3 + 4;

        {
            auto const components = timing_utils::to_floored_components(-frame, timing_unit_kind::fraction,
                                                                        timing_fraction_kind::sample, sample_rate);
            XCTAssertTrue(components.is_minus());
            XCTAssertEqual(components.value(timing_unit_kind::hours), 1);
            XCTAssertEqual(components.value(timing_unit_kind::minutes), 2);
            XCTAssertEqual(components.value(timing_unit_kind::seconds), 3);
            XCTAssertEqual(components.value(timing_unit_kind::fraction), 4);
        }

        {
            auto const components = timing_utils::to_floored_components(-frame, timing_unit_kind::seconds,
                                                                        timing_fraction_kind::sample, sample_rate);
            XCTAssertTrue(components.is_minus());
            XCTAssertEqual(components.value(timing_unit_kind::hours), 1);
            XCTAssertEqual(components.value(timing_unit_kind::minutes), 2);
            XCTAssertEqual(components.value(timing_unit_kind::seconds), 4);
            XCTAssertEqual(components.value(timing_unit_kind::fraction), 0);
        }

        {
            auto const components = timing_utils::to_floored_components(-frame, timing_unit_kind::minutes,
                                                                        timing_fraction_kind::sample, sample_rate);
            XCTAssertTrue(components.is_minus());
            XCTAssertEqual(components.value(timing_unit_kind::hours), 1);
            XCTAssertEqual(components.value(timing_unit_kind::minutes), 3);
            XCTAssertEqual(components.value(timing_unit_kind::seconds), 0);
            XCTAssertEqual(components.value(timing_unit_kind::fraction), 0);
        }

        {
            auto const components = timing_utils::to_floored_components(-frame, timing_unit_kind::hours,
                                                                        timing_fraction_kind::sample, sample_rate);
            XCTAssertTrue(components.is_minus());
            XCTAssertEqual(components.value(timing_unit_kind::hours), 2);
            XCTAssertEqual(components.value(timing_unit_kind::minutes), 0);
            XCTAssertEqual(components.value(timing_unit_kind::seconds), 0);
            XCTAssertEqual(components.value(timing_unit_kind::fraction), 0);
        }
    }

    {
        frame_index_t const frame =
            60 * 60 * sample_rate * 1 + 60 * sample_rate * 2 + sample_rate * 3 + sample_rate / 1000 * 4;

        {
            auto const components = timing_utils::to_floored_components(frame, timing_unit_kind::fraction,
                                                                        timing_fraction_kind::milisecond, sample_rate);
            XCTAssertFalse(components.is_minus());
            XCTAssertEqual(components.value(timing_unit_kind::hours), 1);
            XCTAssertEqual(components.value(timing_unit_kind::minutes), 2);
            XCTAssertEqual(components.value(timing_unit_kind::seconds), 3);
            XCTAssertEqual(components.value(timing_unit_kind::fraction), 4);
        }

        {
            auto const components = timing_utils::to_floored_components(-frame, timing_unit_kind::fraction,
                                                                        timing_fraction_kind::milisecond, sample_rate);
            XCTAssertTrue(components.is_minus());
            XCTAssertEqual(components.value(timing_unit_kind::hours), 1);
            XCTAssertEqual(components.value(timing_unit_kind::minutes), 2);
            XCTAssertEqual(components.value(timing_unit_kind::seconds), 3);
            XCTAssertEqual(components.value(timing_unit_kind::fraction), 4);
        }

        {
            auto const components = timing_utils::to_floored_components(-frame + 1, timing_unit_kind::fraction,
                                                                        timing_fraction_kind::milisecond, sample_rate);
            XCTAssertTrue(components.is_minus());
            XCTAssertEqual(components.value(timing_unit_kind::hours), 1);
            XCTAssertEqual(components.value(timing_unit_kind::minutes), 2);
            XCTAssertEqual(components.value(timing_unit_kind::seconds), 3);
            XCTAssertEqual(components.value(timing_unit_kind::fraction), 4);
        }

        {
            auto const components = timing_utils::to_floored_components(-frame - 1, timing_unit_kind::fraction,
                                                                        timing_fraction_kind::milisecond, sample_rate);
            XCTAssertTrue(components.is_minus());
            XCTAssertEqual(components.value(timing_unit_kind::hours), 1);
            XCTAssertEqual(components.value(timing_unit_kind::minutes), 2);
            XCTAssertEqual(components.value(timing_unit_kind::seconds), 3);
            XCTAssertEqual(components.value(timing_unit_kind::fraction), 5);
        }
    }

    {
        frame_index_t const frame =
            60 * 60 * sample_rate * 1 + 60 * sample_rate * 2 + sample_rate * 3 + sample_rate / 30 * 4;

        {
            auto const components = timing_utils::to_floored_components(frame, timing_unit_kind::fraction,
                                                                        timing_fraction_kind::frame30, sample_rate);
            XCTAssertFalse(components.is_minus());
            XCTAssertEqual(components.value(timing_unit_kind::hours), 1);
            XCTAssertEqual(components.value(timing_unit_kind::minutes), 2);
            XCTAssertEqual(components.value(timing_unit_kind::seconds), 3);
            XCTAssertEqual(components.value(timing_unit_kind::fraction), 4);
        }

        {
            auto const components = timing_utils::to_floored_components(-frame, timing_unit_kind::fraction,
                                                                        timing_fraction_kind::frame30, sample_rate);
            XCTAssertTrue(components.is_minus());
            XCTAssertEqual(components.value(timing_unit_kind::hours), 1);
            XCTAssertEqual(components.value(timing_unit_kind::minutes), 2);
            XCTAssertEqual(components.value(timing_unit_kind::seconds), 3);
            XCTAssertEqual(components.value(timing_unit_kind::fraction), 4);
        }

        {
            auto const components = timing_utils::to_floored_components(-frame + 1, timing_unit_kind::fraction,
                                                                        timing_fraction_kind::frame30, sample_rate);
            XCTAssertTrue(components.is_minus());
            XCTAssertEqual(components.value(timing_unit_kind::hours), 1);
            XCTAssertEqual(components.value(timing_unit_kind::minutes), 2);
            XCTAssertEqual(components.value(timing_unit_kind::seconds), 3);
            XCTAssertEqual(components.value(timing_unit_kind::fraction), 4);
        }

        {
            auto const components = timing_utils::to_floored_components(-frame - 1, timing_unit_kind::fraction,
                                                                        timing_fraction_kind::frame30, sample_rate);
            XCTAssertTrue(components.is_minus());
            XCTAssertEqual(components.value(timing_unit_kind::hours), 1);
            XCTAssertEqual(components.value(timing_unit_kind::minutes), 2);
            XCTAssertEqual(components.value(timing_unit_kind::seconds), 3);
            XCTAssertEqual(components.value(timing_unit_kind::fraction), 5);
        }
    }
}

- (void)test_to_frame {
    sample_rate_t const sample_rate = 48000;

    {
        frame_index_t const expected_frame = 60 * 60 * sample_rate * 1 + 60 * sample_rate * 2 + sample_rate * 3 + 4;
        XCTAssertEqual(timing_utils::to_frame({{.is_minus = false,
                                                .hours = 1,
                                                .minutes = 2,
                                                .seconds = 3,
                                                .fraction = 4,
                                                .fraction_unit_size = sample_rate}},
                                              timing_fraction_kind::sample, sample_rate),
                       expected_frame);
    }

    {
        frame_index_t const expected_frame =
            -(frame_index_t)(60 * 60 * sample_rate * 1 + 60 * sample_rate * 2 + sample_rate * 3 + 4);
        XCTAssertEqual(timing_utils::to_frame({{.is_minus = true,
                                                .hours = 1,
                                                .minutes = 2,
                                                .seconds = 3,
                                                .fraction = 4,
                                                .fraction_unit_size = sample_rate}},
                                              timing_fraction_kind::sample, sample_rate),
                       expected_frame);
    }

    {
        frame_index_t const expected_frame =
            60 * 60 * sample_rate * 1 + 60 * sample_rate * 2 + sample_rate * 3 + (sample_rate / 1000) * 4;
        XCTAssertEqual(timing_utils::to_frame({{.is_minus = false,
                                                .hours = 1,
                                                .minutes = 2,
                                                .seconds = 3,
                                                .fraction = 4,
                                                .fraction_unit_size = 1000}},
                                              timing_fraction_kind::milisecond, sample_rate),
                       expected_frame);
    }

    {
        frame_index_t const expected_frame =
            60 * 60 * sample_rate * 1 + 60 * sample_rate * 2 + sample_rate * 3 + (sample_rate / 30) * 4;
        XCTAssertEqual(
            timing_utils::to_frame(
                {{.is_minus = false, .hours = 1, .minutes = 2, .seconds = 3, .fraction = 4, .fraction_unit_size = 30}},
                timing_fraction_kind::frame30, sample_rate),
            expected_frame);
    }
}

@end

//
//  ae_timing_components_tests.mm
//

#import <XCTest/XCTest.h>
#include <audio_editor_core/ae_timing_types.h>

using namespace yas;
using namespace yas::ae;

@interface ae_timing_components_tests : XCTestCase

@end

@implementation ae_timing_components_tests

- (void)test_construct_with_number_components {
    number_components const number_components{true,
                                              {{.size = 10000, .value = 6789},
                                               {.size = 60, .value = 45},
                                               {.size = 60, .value = 23},
                                               {.size = 100, .value = 1}}};

    timing_components const components{number_components};

    XCTAssertTrue(components.is_minus());
    XCTAssertEqual(components.value(timing_unit_kind::hours), 1);
    XCTAssertEqual(components.value(timing_unit_kind::minutes), 23);
    XCTAssertEqual(components.value(timing_unit_kind::seconds), 45);
    XCTAssertEqual(components.value(timing_unit_kind::fraction), 6789);
    XCTAssertEqual(components.fraction_unit_size(), 10000);

    XCTAssertEqual(components.raw_components(), number_components);
}

- (void)test_construct_with_args {
    timing_components const components{
        {.is_minus = true, .hours = 1, .minutes = 23, .seconds = 45, .fraction = 6789, .fraction_unit_size = 10000}};

    XCTAssertTrue(components.is_minus());
    XCTAssertEqual(components.value(timing_unit_kind::hours), 1);
    XCTAssertEqual(components.value(timing_unit_kind::minutes), 23);
    XCTAssertEqual(components.value(timing_unit_kind::seconds), 45);
    XCTAssertEqual(components.value(timing_unit_kind::fraction), 6789);
    XCTAssertEqual(components.fraction_unit_size(), 10000);

    XCTAssertEqual(components.raw_components(), (number_components{true,
                                                                   {{.size = 10000, .value = 6789},
                                                                    {.size = 60, .value = 45},
                                                                    {.size = 60, .value = 23},
                                                                    {.size = 100, .value = 1}}}));
}

- (void)test_timing_components_is_zero {
    XCTAssertTrue(
        (timing_components{
             {.is_minus = false, .hours = 0, .minutes = 0, .seconds = 0, .fraction = 0, .fraction_unit_size = 10000}})
            .is_zero());
    XCTAssertTrue(
        (timing_components{
             {.is_minus = false, .hours = 0, .minutes = 0, .seconds = 0, .fraction = 0, .fraction_unit_size = 10000}})
            .is_zero());
    XCTAssertTrue(
        (timing_components{
             {.is_minus = true, .hours = 0, .minutes = 0, .seconds = 0, .fraction = 0, .fraction_unit_size = 10000}})
            .is_zero());

    XCTAssertFalse(
        (timing_components{
             {.is_minus = false, .hours = 0, .minutes = 0, .seconds = 0, .fraction = 1, .fraction_unit_size = 10000}})
            .is_zero());
    XCTAssertFalse(
        (timing_components{
             {.is_minus = false, .hours = 0, .minutes = 0, .seconds = 1, .fraction = 0, .fraction_unit_size = 10000}})
            .is_zero());
    XCTAssertFalse(
        (timing_components{
             {.is_minus = false, .hours = 0, .minutes = 1, .seconds = 0, .fraction = 0, .fraction_unit_size = 10000}})
            .is_zero());
    XCTAssertFalse(
        (timing_components{
             {.is_minus = false, .hours = 1, .minutes = 0, .seconds = 0, .fraction = 0, .fraction_unit_size = 10000}})
            .is_zero());
}

- (void)test_abs {
    XCTAssertTrue((timing_components{{.is_minus = true,
                                      .hours = 1,
                                      .minutes = 23,
                                      .seconds = 45,
                                      .fraction = 6789,
                                      .fraction_unit_size = 10000}})
                      .abs() == (timing_components{{.is_minus = false,
                                                    .hours = 1,
                                                    .minutes = 23,
                                                    .seconds = 45,
                                                    .fraction = 6789,
                                                    .fraction_unit_size = 10000}}));
    XCTAssertTrue((timing_components{{.is_minus = false,
                                      .hours = 1,
                                      .minutes = 23,
                                      .seconds = 45,
                                      .fraction = 6789,
                                      .fraction_unit_size = 10000}})
                      .abs() == (timing_components{{.is_minus = false,
                                                    .hours = 1,
                                                    .minutes = 23,
                                                    .seconds = 45,
                                                    .fraction = 6789,
                                                    .fraction_unit_size = 10000}}));
}

- (void)test_timing_components_equal {
    XCTAssertTrue(
        (timing_components{
            {.is_minus = false, .hours = 0, .minutes = 0, .seconds = 0, .fraction = 0, .fraction_unit_size = 10000}}) ==
        (timing_components{
            {.is_minus = false, .hours = 0, .minutes = 0, .seconds = 0, .fraction = 0, .fraction_unit_size = 10000}}));
    XCTAssertTrue(
        (timing_components{
            {.is_minus = false, .hours = 0, .minutes = 0, .seconds = 0, .fraction = 0, .fraction_unit_size = 10000}}) ==
        (timing_components{
            {.is_minus = true, .hours = 0, .minutes = 0, .seconds = 0, .fraction = 0, .fraction_unit_size = 10000}}));

    XCTAssertTrue((timing_components{{.is_minus = true,
                                      .hours = 1,
                                      .minutes = 23,
                                      .seconds = 45,
                                      .fraction = 6789,
                                      .fraction_unit_size = 10000}}) ==
                  (timing_components{{.is_minus = true,
                                      .hours = 1,
                                      .minutes = 23,
                                      .seconds = 45,
                                      .fraction = 6789,
                                      .fraction_unit_size = 10000}}));

    XCTAssertFalse((timing_components{{.is_minus = true,
                                       .hours = 1,
                                       .minutes = 23,
                                       .seconds = 45,
                                       .fraction = 6789,
                                       .fraction_unit_size = 10000}}) ==
                   (timing_components{{.is_minus = false,
                                       .hours = 1,
                                       .minutes = 23,
                                       .seconds = 45,
                                       .fraction = 6789,
                                       .fraction_unit_size = 10000}}));
    XCTAssertFalse((timing_components{{.is_minus = true,
                                       .hours = 1,
                                       .minutes = 23,
                                       .seconds = 45,
                                       .fraction = 6789,
                                       .fraction_unit_size = 10000}}) ==
                   (timing_components{{.is_minus = true,
                                       .hours = 0,
                                       .minutes = 23,
                                       .seconds = 45,
                                       .fraction = 6789,
                                       .fraction_unit_size = 10000}}));
    XCTAssertFalse((timing_components{{.is_minus = true,
                                       .hours = 1,
                                       .minutes = 23,
                                       .seconds = 45,
                                       .fraction = 6789,
                                       .fraction_unit_size = 10000}}) ==
                   (timing_components{{.is_minus = true,
                                       .hours = 1,
                                       .minutes = 0,
                                       .seconds = 45,
                                       .fraction = 6789,
                                       .fraction_unit_size = 10000}}));
    XCTAssertFalse((timing_components{{.is_minus = true,
                                       .hours = 1,
                                       .minutes = 23,
                                       .seconds = 45,
                                       .fraction = 6789,
                                       .fraction_unit_size = 10000}}) ==
                   (timing_components{{.is_minus = true,
                                       .hours = 1,
                                       .minutes = 23,
                                       .seconds = 0,
                                       .fraction = 6789,
                                       .fraction_unit_size = 10000}}));
    XCTAssertFalse(
        (timing_components{{.is_minus = true,
                            .hours = 1,
                            .minutes = 23,
                            .seconds = 45,
                            .fraction = 6789,
                            .fraction_unit_size = 10000}}) ==
        (timing_components{
            {.is_minus = true, .hours = 1, .minutes = 23, .seconds = 45, .fraction = 0, .fraction_unit_size = 10000}}));
}

- (void)test_components_less_than {
    XCTAssertTrue(
        (timing_components{
            {.is_minus = false, .hours = 0, .minutes = 0, .seconds = 0, .fraction = 0, .fraction_unit_size = 100000}}) <
        (timing_components{
            {.is_minus = false, .hours = 0, .minutes = 0, .seconds = 0, .fraction = 1, .fraction_unit_size = 100000}}));
    XCTAssertTrue(
        (timing_components{{.is_minus = false,
                            .hours = 0,
                            .minutes = 0,
                            .seconds = 0,
                            .fraction = 99999,
                            .fraction_unit_size = 100000}}) <
        (timing_components{
            {.is_minus = false, .hours = 0, .minutes = 0, .seconds = 1, .fraction = 0, .fraction_unit_size = 100000}}));
    XCTAssertTrue(
        (timing_components{{.is_minus = false,
                            .hours = 0,
                            .minutes = 0,
                            .seconds = 59,
                            .fraction = 0,
                            .fraction_unit_size = 100000}}) <
        (timing_components{
            {.is_minus = false, .hours = 0, .minutes = 1, .seconds = 0, .fraction = 0, .fraction_unit_size = 100000}}));
    XCTAssertTrue(
        (timing_components{{.is_minus = false,
                            .hours = 0,
                            .minutes = 59,
                            .seconds = 0,
                            .fraction = 0,
                            .fraction_unit_size = 100000}}) <
        (timing_components{
            {.is_minus = false, .hours = 1, .minutes = 0, .seconds = 0, .fraction = 0, .fraction_unit_size = 100000}}));

    XCTAssertFalse(
        (timing_components{
            {.is_minus = true, .hours = 0, .minutes = 0, .seconds = 0, .fraction = 0, .fraction_unit_size = 100000}}) <
        (timing_components{
            {.is_minus = true, .hours = 0, .minutes = 0, .seconds = 0, .fraction = 1, .fraction_unit_size = 100000}}));
    XCTAssertFalse(
        (timing_components{{.is_minus = true,
                            .hours = 0,
                            .minutes = 0,
                            .seconds = 0,
                            .fraction = 99999,
                            .fraction_unit_size = 100000}}) <
        (timing_components{
            {.is_minus = true, .hours = 0, .minutes = 0, .seconds = 1, .fraction = 0, .fraction_unit_size = 100000}}));
    XCTAssertFalse(
        (timing_components{
            {.is_minus = true, .hours = 0, .minutes = 0, .seconds = 59, .fraction = 0, .fraction_unit_size = 100000}}) <
        (timing_components{
            {.is_minus = true, .hours = 0, .minutes = 1, .seconds = 0, .fraction = 0, .fraction_unit_size = 100000}}));
    XCTAssertFalse(
        (timing_components{
            {.is_minus = true, .hours = 0, .minutes = 59, .seconds = 0, .fraction = 0, .fraction_unit_size = 100000}}) <
        (timing_components{
            {.is_minus = true, .hours = 1, .minutes = 0, .seconds = 0, .fraction = 0, .fraction_unit_size = 100000}}));

    XCTAssertTrue(
        (timing_components{
            {.is_minus = true, .hours = 0, .minutes = 0, .seconds = 0, .fraction = 2, .fraction_unit_size = 100000}}) <
        (timing_components{
            {.is_minus = false, .hours = 0, .minutes = 0, .seconds = 0, .fraction = 1, .fraction_unit_size = 100000}}));
    XCTAssertFalse(
        (timing_components{
            {.is_minus = false, .hours = 0, .minutes = 0, .seconds = 0, .fraction = 1, .fraction_unit_size = 100000}}) <
        (timing_components{
            {.is_minus = true, .hours = 0, .minutes = 0, .seconds = 0, .fraction = 2, .fraction_unit_size = 100000}}));
}

@end

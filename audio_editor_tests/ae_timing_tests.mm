//
//  ae_timing_tests.mm
//

#import <XCTest/XCTest.h>
#include <audio_editor_core/ae_timing.h>

using namespace yas;
using namespace yas::ae;

@interface ae_timing_tests : XCTestCase

@end

@implementation ae_timing_tests

- (void)test_sample_rate {
    XCTAssertEqual(timing::make_shared(1)->sample_rate(), 1);
    XCTAssertEqual(timing::make_shared(44100)->sample_rate(), 44100);
    XCTAssertEqual(timing::make_shared(48000)->sample_rate(), 48000);
    XCTAssertEqual(timing::make_shared(96000)->sample_rate(), 96000);
}

- (void)test_set_fraction_kind {
    auto const timing = timing::make_shared(48000);

    std::vector<timing_fraction_kind> called;

    auto canceller =
        timing->observe_fraction_kind([&called](timing_fraction_kind const &kind) { called.emplace_back(kind); })
            .sync();

    XCTAssertEqual(called.size(), 1);
    XCTAssertEqual(called.at(0), timing_fraction_kind::sample);
    XCTAssertEqual(timing->fraction_kind(), timing_fraction_kind::sample);

    timing->set_fraction_kind(timing_fraction_kind::milisecond);

    XCTAssertEqual(called.size(), 2);
    XCTAssertEqual(called.at(1), timing_fraction_kind::milisecond);
    XCTAssertEqual(timing->fraction_kind(), timing_fraction_kind::milisecond);

    timing->set_fraction_kind(timing_fraction_kind::frame30);

    XCTAssertEqual(called.size(), 3);
    XCTAssertEqual(called.at(2), timing_fraction_kind::frame30);
    XCTAssertEqual(timing->fraction_kind(), timing_fraction_kind::frame30);

    canceller->cancel();
}

- (void)test_components_fraction_of_sample {
    auto const timing = timing::make_shared(48000);

    timing->set_fraction_kind(timing_fraction_kind::sample);

    XCTAssertEqual(
        timing->components(0),
        (timing_components{
            {.is_minus = false, .hours = 0, .minutes = 0, .seconds = 0, .fraction = 0, .fraction_unit_count = 48000}}));
    XCTAssertEqual(
        timing->components(1),
        (timing_components{
            {.is_minus = false, .hours = 0, .minutes = 0, .seconds = 0, .fraction = 1, .fraction_unit_count = 48000}}));
    XCTAssertEqual(timing->components(47999), (timing_components{{.is_minus = false,
                                                                  .hours = 0,
                                                                  .minutes = 0,
                                                                  .seconds = 0,
                                                                  .fraction = 47999,
                                                                  .fraction_unit_count = 48000}}));
    XCTAssertEqual(
        timing->components(48000),
        (timing_components{
            {.is_minus = false, .hours = 0, .minutes = 0, .seconds = 1, .fraction = 0, .fraction_unit_count = 48000}}));
    XCTAssertEqual(
        timing->components(-1),
        (timing_components{
            {.is_minus = true, .hours = 0, .minutes = 0, .seconds = 0, .fraction = 1, .fraction_unit_count = 48000}}));
    XCTAssertEqual(timing->components(-47999), (timing_components{{.is_minus = true,
                                                                   .hours = 0,
                                                                   .minutes = 0,
                                                                   .seconds = 0,
                                                                   .fraction = 47999,
                                                                   .fraction_unit_count = 48000}}));
    XCTAssertEqual(
        timing->components(-48000),
        (timing_components{
            {.is_minus = true, .hours = 0, .minutes = 0, .seconds = 1, .fraction = 0, .fraction_unit_count = 48000}}));
}

- (void)test_components_fraction_of_milisecond {
    auto const timing = timing::make_shared(48000);

    timing->set_fraction_kind(timing_fraction_kind::milisecond);

    XCTAssertEqual(
        timing->components(0),
        (timing_components{
            {.is_minus = false, .hours = 0, .minutes = 0, .seconds = 0, .fraction = 0, .fraction_unit_count = 1000}}));
    XCTAssertEqual(
        timing->components(1),
        (timing_components{
            {.is_minus = false, .hours = 0, .minutes = 0, .seconds = 0, .fraction = 0, .fraction_unit_count = 1000}}));
    XCTAssertEqual(
        timing->components(47),
        (timing_components{
            {.is_minus = false, .hours = 0, .minutes = 0, .seconds = 0, .fraction = 0, .fraction_unit_count = 1000}}));
    XCTAssertEqual(
        timing->components(48),
        (timing_components{
            {.is_minus = false, .hours = 0, .minutes = 0, .seconds = 0, .fraction = 1, .fraction_unit_count = 1000}}));
    XCTAssertEqual(timing->components(47999), (timing_components{{.is_minus = false,
                                                                  .hours = 0,
                                                                  .minutes = 0,
                                                                  .seconds = 0,
                                                                  .fraction = 999,
                                                                  .fraction_unit_count = 1000}}));
    XCTAssertEqual(
        timing->components(48000),
        (timing_components{
            {.is_minus = false, .hours = 0, .minutes = 0, .seconds = 1, .fraction = 0, .fraction_unit_count = 1000}}));
    XCTAssertEqual(
        timing->components(-1),
        (timing_components{
            {.is_minus = true, .hours = 0, .minutes = 0, .seconds = 0, .fraction = 0, .fraction_unit_count = 1000}}));
    XCTAssertEqual(
        timing->components(-47),
        (timing_components{
            {.is_minus = true, .hours = 0, .minutes = 0, .seconds = 0, .fraction = 0, .fraction_unit_count = 1000}}));
    XCTAssertEqual(
        timing->components(-48),
        (timing_components{
            {.is_minus = true, .hours = 0, .minutes = 0, .seconds = 0, .fraction = 1, .fraction_unit_count = 1000}}));
    XCTAssertEqual(
        timing->components(-47999),
        (timing_components{
            {.is_minus = true, .hours = 0, .minutes = 0, .seconds = 0, .fraction = 999, .fraction_unit_count = 1000}}));
    XCTAssertEqual(
        timing->components(-48000),
        (timing_components{
            {.is_minus = true, .hours = 0, .minutes = 0, .seconds = 1, .fraction = 0, .fraction_unit_count = 1000}}));
}

- (void)test_components_fraction_of_frame30 {
    auto const timing = timing::make_shared(48000);

    timing->set_fraction_kind(timing_fraction_kind::frame30);

    XCTAssertEqual(
        timing->components(0),
        (timing_components{
            {.is_minus = false, .hours = 0, .minutes = 0, .seconds = 0, .fraction = 0, .fraction_unit_count = 30}}));
    XCTAssertEqual(
        timing->components(1),
        (timing_components{
            {.is_minus = false, .hours = 0, .minutes = 0, .seconds = 0, .fraction = 0, .fraction_unit_count = 30}}));
    XCTAssertEqual(
        timing->components(1599),
        (timing_components{
            {.is_minus = false, .hours = 0, .minutes = 0, .seconds = 0, .fraction = 0, .fraction_unit_count = 30}}));
    XCTAssertEqual(
        timing->components(1600),
        (timing_components{
            {.is_minus = false, .hours = 0, .minutes = 0, .seconds = 0, .fraction = 1, .fraction_unit_count = 30}}));
    XCTAssertEqual(
        timing->components(47999),
        (timing_components{
            {.is_minus = false, .hours = 0, .minutes = 0, .seconds = 0, .fraction = 29, .fraction_unit_count = 30}}));
    XCTAssertEqual(
        timing->components(48000),
        (timing_components{
            {.is_minus = false, .hours = 0, .minutes = 0, .seconds = 1, .fraction = 0, .fraction_unit_count = 30}}));
    XCTAssertEqual(
        timing->components(-1),
        (timing_components{
            {.is_minus = true, .hours = 0, .minutes = 0, .seconds = 0, .fraction = 0, .fraction_unit_count = 30}}));
    XCTAssertEqual(
        timing->components(-1599),
        (timing_components{
            {.is_minus = true, .hours = 0, .minutes = 0, .seconds = 0, .fraction = 0, .fraction_unit_count = 30}}));
    XCTAssertEqual(
        timing->components(-1600),
        (timing_components{
            {.is_minus = true, .hours = 0, .minutes = 0, .seconds = 0, .fraction = 1, .fraction_unit_count = 30}}));
    XCTAssertEqual(
        timing->components(-47999),
        (timing_components{
            {.is_minus = true, .hours = 0, .minutes = 0, .seconds = 0, .fraction = 29, .fraction_unit_count = 30}}));
    XCTAssertEqual(
        timing->components(-48000),
        (timing_components{
            {.is_minus = true, .hours = 0, .minutes = 0, .seconds = 1, .fraction = 0, .fraction_unit_count = 30}}));
}

- (void)test_fraction_digits {
    auto const timing = timing::make_shared(48000);

    timing->set_fraction_kind(timing_fraction_kind::sample);

    XCTAssertEqual(timing->fraction_digits(), 5);

    timing->set_fraction_kind(timing_fraction_kind::milisecond);

    XCTAssertEqual(timing->fraction_digits(), 3);

    timing->set_fraction_kind(timing_fraction_kind::frame30);

    XCTAssertEqual(timing->fraction_digits(), 2);
}

- (void)test_frame {
    auto const timing = timing::make_shared(10000);

    timing->set_fraction_kind(timing_fraction_kind::sample);

    {
        auto const frame = timing->frame(
            {{.is_minus = false, .hours = 1, .minutes = 2, .seconds = 3, .fraction = 4, .fraction_unit_count = 10000}});

        XCTAssertEqual(frame, ((10000 * 60 * 60) + (10000 * 60 * 2) + (10000 * 3) + 4));
    }

    XCTAssertEqual(timing->frame(timing->components(76821058)), 76821058);
    XCTAssertEqual(timing->frame(timing->components(-76821058)), -76821058);

    timing->set_fraction_kind(timing_fraction_kind::milisecond);

    XCTAssertEqual(timing->frame(timing->components(76821058)), 76821050);

    timing->set_fraction_kind(timing_fraction_kind::frame30);

    XCTAssertEqual(timing->frame(timing->components(76821058)), 76821000);
}

@end

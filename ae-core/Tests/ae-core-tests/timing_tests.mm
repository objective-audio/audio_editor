//
//  timing_tests.mm
//

#import <XCTest/XCTest.h>
#include <ae-core/project_editing/features/timing.h>

using namespace yas;
using namespace yas::ae;

namespace yas::ae::test_utils {
struct app_settings_stub : app_settings_for_timing {
    observing::value::holder_ptr<ae::timing_fraction_kind> const kind_holder =
        observing::value::holder<ae::timing_fraction_kind>::make_shared(timing_fraction_kind::sample);

    void set_timing_fraction_kind(ae::timing_fraction_kind const kind) override {
        this->kind_holder->set_value(kind);
    }

    ae::timing_fraction_kind timing_fraction_kind() const override {
        return this->kind_holder->value();
    }

    observing::syncable observe_timing_fraction_kind(
        std::function<void(ae::timing_fraction_kind const &)> &&handler) override {
        return this->kind_holder->observe(std::move(handler));
    }
};
}  // namespace yas::ae::test_utils

@interface timing_tests : XCTestCase

@end

@implementation timing_tests

- (void)test_sample_rate {
    auto const app_settings = std::make_shared<test_utils::app_settings_stub>();

    XCTAssertEqual(std::make_shared<ae::timing>(1, app_settings.get())->sample_rate(), 1);
    XCTAssertEqual(std::make_shared<ae::timing>(44100, app_settings.get())->sample_rate(), 44100);
    XCTAssertEqual(std::make_shared<ae::timing>(48000, app_settings.get())->sample_rate(), 48000);
    XCTAssertEqual(std::make_shared<ae::timing>(96000, app_settings.get())->sample_rate(), 96000);
}

- (void)test_rotate_fraction_kind {
    auto const app_settings = std::make_shared<test_utils::app_settings_stub>();
    auto const timing = std::make_shared<ae::timing>(48000, app_settings.get());

    std::vector<timing_fraction_kind> called;

    auto canceller =
        app_settings
            ->observe_timing_fraction_kind([&called](timing_fraction_kind const &kind) { called.emplace_back(kind); })
            .sync();

    XCTAssertEqual(called.size(), 1);
    XCTAssertEqual(called.at(0), timing_fraction_kind::sample);
    XCTAssertEqual(app_settings->timing_fraction_kind(), timing_fraction_kind::sample);

    timing->rotate_fraction();

    XCTAssertEqual(called.size(), 2);
    XCTAssertEqual(called.at(1), timing_fraction_kind::milisecond);
    XCTAssertEqual(app_settings->timing_fraction_kind(), timing_fraction_kind::milisecond);

    timing->rotate_fraction();

    XCTAssertEqual(called.size(), 3);
    XCTAssertEqual(called.at(2), timing_fraction_kind::frame30);
    XCTAssertEqual(app_settings->timing_fraction_kind(), timing_fraction_kind::frame30);

    canceller->cancel();
}

- (void)test_components_fraction_of_sample {
    auto const app_settings = std::make_shared<test_utils::app_settings_stub>();
    auto const timing = std::make_shared<ae::timing>(48000, app_settings.get());

    app_settings->set_timing_fraction_kind(timing_fraction_kind::sample);

    XCTAssertEqual(
        timing->components(0),
        (timing_components{
            {.is_minus = false, .hours = 0, .minutes = 0, .seconds = 0, .fraction = 0, .fraction_unit_size = 48000}}));
    XCTAssertEqual(
        timing->components(1),
        (timing_components{
            {.is_minus = false, .hours = 0, .minutes = 0, .seconds = 0, .fraction = 1, .fraction_unit_size = 48000}}));
    XCTAssertEqual(timing->components(47999), (timing_components{{.is_minus = false,
                                                                  .hours = 0,
                                                                  .minutes = 0,
                                                                  .seconds = 0,
                                                                  .fraction = 47999,
                                                                  .fraction_unit_size = 48000}}));
    XCTAssertEqual(
        timing->components(48000),
        (timing_components{
            {.is_minus = false, .hours = 0, .minutes = 0, .seconds = 1, .fraction = 0, .fraction_unit_size = 48000}}));
    XCTAssertEqual(
        timing->components(-1),
        (timing_components{
            {.is_minus = true, .hours = 0, .minutes = 0, .seconds = 0, .fraction = 1, .fraction_unit_size = 48000}}));
    XCTAssertEqual(timing->components(-47999), (timing_components{{.is_minus = true,
                                                                   .hours = 0,
                                                                   .minutes = 0,
                                                                   .seconds = 0,
                                                                   .fraction = 47999,
                                                                   .fraction_unit_size = 48000}}));
    XCTAssertEqual(
        timing->components(-48000),
        (timing_components{
            {.is_minus = true, .hours = 0, .minutes = 0, .seconds = 1, .fraction = 0, .fraction_unit_size = 48000}}));
}

- (void)test_components_fraction_of_milisecond {
    auto const app_settings = std::make_shared<test_utils::app_settings_stub>();
    auto const timing = std::make_shared<ae::timing>(48000, app_settings.get());

    app_settings->set_timing_fraction_kind(timing_fraction_kind::milisecond);

    XCTAssertEqual(
        timing->components(0),
        (timing_components{
            {.is_minus = false, .hours = 0, .minutes = 0, .seconds = 0, .fraction = 0, .fraction_unit_size = 1000}}));
    XCTAssertEqual(
        timing->components(1),
        (timing_components{
            {.is_minus = false, .hours = 0, .minutes = 0, .seconds = 0, .fraction = 0, .fraction_unit_size = 1000}}));
    XCTAssertEqual(
        timing->components(47),
        (timing_components{
            {.is_minus = false, .hours = 0, .minutes = 0, .seconds = 0, .fraction = 0, .fraction_unit_size = 1000}}));
    XCTAssertEqual(
        timing->components(48),
        (timing_components{
            {.is_minus = false, .hours = 0, .minutes = 0, .seconds = 0, .fraction = 1, .fraction_unit_size = 1000}}));
    XCTAssertEqual(
        timing->components(47999),
        (timing_components{
            {.is_minus = false, .hours = 0, .minutes = 0, .seconds = 0, .fraction = 999, .fraction_unit_size = 1000}}));
    XCTAssertEqual(
        timing->components(48000),
        (timing_components{
            {.is_minus = false, .hours = 0, .minutes = 0, .seconds = 1, .fraction = 0, .fraction_unit_size = 1000}}));
    XCTAssertEqual(
        timing->components(-1),
        (timing_components{
            {.is_minus = true, .hours = 0, .minutes = 0, .seconds = 0, .fraction = 0, .fraction_unit_size = 1000}}));
    XCTAssertEqual(
        timing->components(-47),
        (timing_components{
            {.is_minus = true, .hours = 0, .minutes = 0, .seconds = 0, .fraction = 0, .fraction_unit_size = 1000}}));
    XCTAssertEqual(
        timing->components(-48),
        (timing_components{
            {.is_minus = true, .hours = 0, .minutes = 0, .seconds = 0, .fraction = 1, .fraction_unit_size = 1000}}));
    XCTAssertEqual(
        timing->components(-47999),
        (timing_components{
            {.is_minus = true, .hours = 0, .minutes = 0, .seconds = 0, .fraction = 999, .fraction_unit_size = 1000}}));
    XCTAssertEqual(
        timing->components(-48000),
        (timing_components{
            {.is_minus = true, .hours = 0, .minutes = 0, .seconds = 1, .fraction = 0, .fraction_unit_size = 1000}}));
}

- (void)test_components_fraction_of_frame30 {
    auto const app_settings = std::make_shared<test_utils::app_settings_stub>();
    auto const timing = std::make_shared<ae::timing>(48000, app_settings.get());

    app_settings->set_timing_fraction_kind(timing_fraction_kind::frame30);

    XCTAssertEqual(
        timing->components(0),
        (timing_components{
            {.is_minus = false, .hours = 0, .minutes = 0, .seconds = 0, .fraction = 0, .fraction_unit_size = 30}}));
    XCTAssertEqual(
        timing->components(1),
        (timing_components{
            {.is_minus = false, .hours = 0, .minutes = 0, .seconds = 0, .fraction = 0, .fraction_unit_size = 30}}));
    XCTAssertEqual(
        timing->components(1599),
        (timing_components{
            {.is_minus = false, .hours = 0, .minutes = 0, .seconds = 0, .fraction = 0, .fraction_unit_size = 30}}));
    XCTAssertEqual(
        timing->components(1600),
        (timing_components{
            {.is_minus = false, .hours = 0, .minutes = 0, .seconds = 0, .fraction = 1, .fraction_unit_size = 30}}));
    XCTAssertEqual(
        timing->components(47999),
        (timing_components{
            {.is_minus = false, .hours = 0, .minutes = 0, .seconds = 0, .fraction = 29, .fraction_unit_size = 30}}));
    XCTAssertEqual(
        timing->components(48000),
        (timing_components{
            {.is_minus = false, .hours = 0, .minutes = 0, .seconds = 1, .fraction = 0, .fraction_unit_size = 30}}));
    XCTAssertEqual(
        timing->components(-1),
        (timing_components{
            {.is_minus = true, .hours = 0, .minutes = 0, .seconds = 0, .fraction = 0, .fraction_unit_size = 30}}));
    XCTAssertEqual(
        timing->components(-1599),
        (timing_components{
            {.is_minus = true, .hours = 0, .minutes = 0, .seconds = 0, .fraction = 0, .fraction_unit_size = 30}}));
    XCTAssertEqual(
        timing->components(-1600),
        (timing_components{
            {.is_minus = true, .hours = 0, .minutes = 0, .seconds = 0, .fraction = 1, .fraction_unit_size = 30}}));
    XCTAssertEqual(
        timing->components(-47999),
        (timing_components{
            {.is_minus = true, .hours = 0, .minutes = 0, .seconds = 0, .fraction = 29, .fraction_unit_size = 30}}));
    XCTAssertEqual(
        timing->components(-48000),
        (timing_components{
            {.is_minus = true, .hours = 0, .minutes = 0, .seconds = 1, .fraction = 0, .fraction_unit_size = 30}}));
}

- (void)test_fraction_digits {
    auto const app_settings = std::make_shared<test_utils::app_settings_stub>();
    auto const timing = std::make_shared<ae::timing>(48000, app_settings.get());

    app_settings->set_timing_fraction_kind(timing_fraction_kind::sample);

    XCTAssertEqual(timing->fraction_digits(), 5);

    app_settings->set_timing_fraction_kind(timing_fraction_kind::milisecond);

    XCTAssertEqual(timing->fraction_digits(), 3);

    app_settings->set_timing_fraction_kind(timing_fraction_kind::frame30);

    XCTAssertEqual(timing->fraction_digits(), 2);
}

- (void)test_frame {
    auto const app_settings = std::make_shared<test_utils::app_settings_stub>();
    auto const timing = std::make_shared<ae::timing>(10000, app_settings.get());

    app_settings->set_timing_fraction_kind(timing_fraction_kind::sample);

    {
        auto const frame = timing->frame(
            {{.is_minus = false, .hours = 1, .minutes = 2, .seconds = 3, .fraction = 4, .fraction_unit_size = 10000}});

        XCTAssertEqual(frame, ((10000 * 60 * 60) + (10000 * 60 * 2) + (10000 * 3) + 4));
    }

    XCTAssertEqual(timing->frame(timing->components(76821058)), 76821058);
    XCTAssertEqual(timing->frame(timing->components(-76821058)), -76821058);

    app_settings->set_timing_fraction_kind(timing_fraction_kind::milisecond);

    XCTAssertEqual(timing->frame(timing->components(76821058)), 76821050);

    app_settings->set_timing_fraction_kind(timing_fraction_kind::frame30);

    XCTAssertEqual(timing->frame(timing->components(76821058)), 76821000);
}

- (void)test_offset {
    XCTAssertEqual(
        timing_components::offset({.is_minus = false, .count = 1, .unit_index = 2, .fraction_unit_size = 10000}),
        (timing_components{{.is_minus = false, .minutes = 1, .fraction_unit_size = 10000}}));
    XCTAssertEqual(
        timing_components::offset({.is_minus = false, .count = 10, .unit_index = 1, .fraction_unit_size = 10000}),
        (timing_components{{.is_minus = false, .seconds = 10, .fraction_unit_size = 10000}}));
    XCTAssertEqual(
        timing_components::offset({.is_minus = false, .count = 100, .unit_index = 0, .fraction_unit_size = 10000}),
        (timing_components{{.is_minus = false, .fraction = 100, .fraction_unit_size = 10000}}));

    // マイナス
    XCTAssertEqual(
        timing_components::offset({.is_minus = true, .count = 1, .unit_index = 2, .fraction_unit_size = 10000}),
        (timing_components{{.is_minus = true, .minutes = 1, .fraction_unit_size = 10000}}));
    XCTAssertEqual(
        timing_components::offset({.is_minus = true, .count = 10, .unit_index = 1, .fraction_unit_size = 10000}),
        (timing_components{{.is_minus = true, .seconds = 10, .fraction_unit_size = 10000}}));
    XCTAssertEqual(
        timing_components::offset({.is_minus = true, .count = 100, .unit_index = 0, .fraction_unit_size = 10000}),
        (timing_components{{.is_minus = true, .fraction = 100, .fraction_unit_size = 10000}}));

    // 繰り上がり
    XCTAssertEqual(
        timing_components::offset({.is_minus = false, .count = 623456, .unit_index = 0, .fraction_unit_size = 10000}),
        (timing_components{
            {.is_minus = false, .minutes = 1, .seconds = 2, .fraction = 3456, .fraction_unit_size = 10000}}));
}

@end

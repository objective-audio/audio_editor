//
//  ae_display_space_tests.mm
//

#import <XCTest/XCTest.h>
#include <audio_editor_core/ae_display_space.h>

using namespace yas;
using namespace yas::ae;

@interface ae_display_space_tests : XCTestCase

@end

@implementation ae_display_space_tests

- (void)test_initial {
    {
        auto const space = display_space::make_shared(ui::region::zero());
        XCTAssertTrue(space->region() == ui::region::zero());
    }

    {
        auto const region = ui::region{.origin = {.x = -1.0, .y = -2.0}, .size = {.width = 2.0, .height = 4.0}};
        auto const space = display_space::make_shared(region);
        XCTAssertTrue(space->region() == region);
    }
}

- (void)test_scaled_region {
    auto const region = ui::region{.origin = {.x = -1.0, .y = -2.0}, .size = {.width = 2.0, .height = 4.0}};
    auto const space = display_space::make_shared(region);

    {
        space->set_scale({0.5, 0.25});

        auto scaled_region = space->region();
        XCTAssertEqualWithAccuracy(scaled_region.origin.x, -2.0, 0.001);
        XCTAssertEqualWithAccuracy(scaled_region.origin.y, -8.0, 0.001);
        XCTAssertEqualWithAccuracy(scaled_region.size.width, 4.0, 0.001);
        XCTAssertEqualWithAccuracy(scaled_region.size.height, 16.0, 0.001);
    }

    {
        space->set_scale({2.0, 8.0});

        auto scaled_region = space->region();
        XCTAssertEqualWithAccuracy(scaled_region.origin.x, -0.5, 0.001);
        XCTAssertEqualWithAccuracy(scaled_region.origin.y, -0.25, 0.001);
        XCTAssertEqualWithAccuracy(scaled_region.size.width, 1.0, 0.001);
        XCTAssertEqualWithAccuracy(scaled_region.size.height, 0.5, 0.001);
    }
}

- (void)test_observe {
    auto const space = display_space::make_shared(ui::region::zero());

    struct called_event {
        display_space_event_source source;
        ui::region region;
        ui::size scale;
    };

    std::vector<called_event> called;

    auto const canceller =
        space
            ->observe([&called](display_space_event const &event) {
                called.emplace_back(called_event{.source = event.source, .region = event.region, .scale = event.scale});
            })
            .sync();

    XCTAssertEqual(called.size(), 1);
    XCTAssertTrue(called.at(0).source == display_space_event_source::fetched);
    XCTAssertTrue(called.at(0).region == ui::region::zero());
    XCTAssertTrue(called.at(0).scale == (ui::size{1.0f, 1.0f}));

    space->set_view_region(ui::region::zero());
    XCTAssertEqual(called.size(), 1);

    auto const view_region = ui::region{.size = {.width = 1.0, .height = 1.0}};
    space->set_view_region(view_region);

    XCTAssertEqual(called.size(), 2);

    XCTAssertTrue(called.at(1).source == display_space_event_source::view);
    XCTAssertTrue(called.at(1).region == view_region);
    XCTAssertTrue(called.at(1).scale == (ui::size{1.0f, 1.0f}));

    auto const scale = ui::size{2.0, 4.0};
    space->set_scale(scale);

    XCTAssertEqual(called.size(), 3);
    XCTAssertTrue(called.at(2).source == display_space_event_source::scale);
    auto const expected_region = ui::region{.size = {.width = 0.5, .height = 0.25}};
    XCTAssertTrue(called.at(2).region == expected_region);
    XCTAssertTrue(called.at(2).scale == scale);

    canceller->cancel();
}

@end

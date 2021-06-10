//
//  ae_ui_layout_utils_tests.mm
//

#import <XCTest/XCTest.h>
#import <audio_editor_core/audio_editor_core_umbrella.h>

using namespace yas;
using namespace yas::ae;

@interface ae_ui_layout_utils_tests : XCTestCase

@end

@implementation ae_ui_layout_utils_tests

- (void)test_constant_value {
    XCTAssertEqual(ui_layout_utils::constant(0.0f)(0.0f), 0.0f);
    XCTAssertEqual(ui_layout_utils::constant(0.0f)(1.0f), 1.0f);
    XCTAssertEqual(ui_layout_utils::constant(0.0f)(-1.0f), -1.0f);

    XCTAssertEqual(ui_layout_utils::constant(1.0f)(0.0f), 1.0f);
    XCTAssertEqual(ui_layout_utils::constant(1.0f)(1.0f), 2.0f);
    XCTAssertEqual(ui_layout_utils::constant(1.0f)(-1.0f), 0.0f);
}

- (void)test_constant_point {
    XCTAssertTrue(ui_layout_utils::constant(ui::point::zero())({0.0f, 0.0f}) == ui::point::zero());
    XCTAssertTrue(ui_layout_utils::constant(ui::point::zero())({1.0f, 2.0f}) == (ui::point{1.0f, 2.0f}));
    XCTAssertTrue(ui_layout_utils::constant(ui::point::zero())({-1.0f, -2.0f}) == (ui::point{-1.0f, -2.0f}));

    XCTAssertTrue(ui_layout_utils::constant(ui::point{1.0f, 2.0f})({0.0f, 0.0f}) == (ui::point{1.0f, 2.0f}));
    XCTAssertTrue(ui_layout_utils::constant(ui::point{1.0f, 2.0f})({1.0f, 2.0f}) == (ui::point{2.0f, 4.0f}));
    XCTAssertTrue(ui_layout_utils::constant(ui::point{1.0f, 2.0f})({-1.0f, -2.0f}) == (ui::point{0.0f, 0.0f}));
}

- (void)test_constant_range {
    XCTAssertTrue(ui_layout_utils::constant(ui::range_insets::zero())({0.0f, 0.0f}) == ui::range::zero());
    XCTAssertTrue(ui_layout_utils::constant(ui::range_insets::zero())({1.0f, 2.0f}) == (ui::range{1.0f, 2.0f}));
    XCTAssertTrue(ui_layout_utils::constant(ui::range_insets::zero())({-1.0f, 3.0f}) == (ui::range{-1.0f, 3.0f}));

    XCTAssertTrue(ui_layout_utils::constant(ui::range_insets{1.0f, 2.0f})({0.0f, 0.0f}) == (ui::range{1.0f, 1.0f}));
    XCTAssertTrue(ui_layout_utils::constant(ui::range_insets{1.0f, 2.0f})({1.0f, 2.0f}) == (ui::range{2.0f, 3.0f}));
    XCTAssertTrue(ui_layout_utils::constant(ui::range_insets{1.0f, 2.0f})({-1.0f, 3.0f}) == (ui::range{0.0f, 4.0f}));
}

- (void)test_constant_region {
    XCTAssertTrue(ui_layout_utils::constant(ui::region_insets::zero())(ui::region::zero()) == ui::region::zero());
    XCTAssertTrue(ui_layout_utils::constant(ui::region_insets::zero())(
                      ui::region{.origin = {1.0f, 2.0f}, .size = {3.0f, 4.0f}}) ==
                  (ui::region{.origin = {1.0f, 2.0f}, .size = {3.0f, 4.0f}}));

    XCTAssertTrue(ui_layout_utils::constant(ui::region_insets{-1.0f, 1.0f, -1.0f, 1.0f})(ui::region::zero()) ==
                  (ui::region{.origin = {-1.0f, -1.0f}, .size = {2.0f, 2.0f}}));
    XCTAssertTrue(ui_layout_utils::constant(ui::region_insets{-1.0f, 1.0f, -1.0f, 1.0f})(
                      ui::region{.origin = {1.0f, 2.0f}, .size = {3.0f, 4.0f}}) ==
                  (ui::region{.origin = {0.0f, 1.0f}, .size = {5.0f, 6.0f}}));
}

@end

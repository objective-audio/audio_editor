//
//  ae_point_range_tests.mm
//

#import <XCTest/XCTest.h>
#include <ae-core/project_editing/value_types/ae_point_range.hpp>

using namespace yas;
using namespace yas::ae;

@interface ae_point_range_tests : XCTestCase

@end

@implementation ae_point_range_tests

- (void)test_region {
    XCTAssertTrue((point_range{.first = ui::point::zero(), .second = ui::point::zero()}.region()) ==
                  ui::region::zero());
    XCTAssertTrue((point_range{.first = {.x = 1.0f, .y = 2.0f}, .second = {.x = 3.0f, .y = 6.0f}}.region()) ==
                  (ui::region{.origin = {.x = 1.0f, .y = 2.0f}, .size = {.width = 2.0f, .height = 4.0f}}));
    XCTAssertTrue((point_range{.first = {.x = 1.0f, .y = 2.0f}, .second = {.x = -3.0f, .y = -6.0f}}.region()) ==
                  (ui::region{.origin = {.x = -3.0f, .y = -6.0f}, .size = {.width = 4.0f, .height = 8.0f}}));
}

- (void)test_second_updated {
    XCTAssertTrue((point_range{.first = {.x = 1.0f, .y = 2.0f}, .second = {.x = 3.0f, .y = 4.0f}}.second_updated(
                      {.x = 10.0f, .y = 11.0f})) ==
                  (point_range{.first = {.x = 1.0f, .y = 2.0f}, .second = {.x = 10.0f, .y = 11.0f}}));
}

- (void)test_equal {
    point_range const range1a{.first = {.x = 1.0f, .y = 2.0f}, .second = {.x = 3.0f, .y = 4.0f}};
    point_range const range1b{.first = {.x = 1.0f, .y = 2.0f}, .second = {.x = 3.0f, .y = 4.0f}};
    point_range const range2{.first = {.x = 1.0f, .y = 10.0f}, .second = {.x = 3.0f, .y = 4.0f}};
    point_range const range3{.first = {.x = 1.0f, .y = 2.0f}, .second = {.x = 11.0f, .y = 4.0f}};

    XCTAssertTrue(range1a == range1a);
    XCTAssertTrue(range1a == range1b);
    XCTAssertFalse(range1a == range2);
    XCTAssertFalse(range1a == range3);

    XCTAssertFalse(range1a != range1a);
    XCTAssertFalse(range1a != range1b);
    XCTAssertTrue(range1a != range2);
    XCTAssertTrue(range1a != range3);
}

@end

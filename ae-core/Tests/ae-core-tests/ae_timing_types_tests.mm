//
//  ae_timing_types_tests.mm
//

#import <XCTest/XCTest.h>
#include <ae-core/project_editing/value_types/ae_timing_types.h>

using namespace yas;
using namespace yas::ae;

@interface ae_timing_types_tests : XCTestCase

@end

@implementation ae_timing_types_tests

- (void)test_timing_unit_kind_to_index {
    XCTAssertEqual(to_index(timing_unit_kind::fraction), 0);
    XCTAssertEqual(to_index(timing_unit_kind::seconds), 1);
    XCTAssertEqual(to_index(timing_unit_kind::minutes), 2);
    XCTAssertEqual(to_index(timing_unit_kind::hours), 3);
}

- (void)test_timing_unit_kind_rotate_next {
    XCTAssertEqual(rotate_next(timing_unit_kind::fraction), timing_unit_kind::hours);
    XCTAssertEqual(rotate_next(timing_unit_kind::seconds), timing_unit_kind::fraction);
    XCTAssertEqual(rotate_next(timing_unit_kind::minutes), timing_unit_kind::seconds);
    XCTAssertEqual(rotate_next(timing_unit_kind::hours), timing_unit_kind::minutes);
}

- (void)test_timing_unit_kind_rotate_previous {
    XCTAssertEqual(rotate_previous(timing_unit_kind::fraction), timing_unit_kind::seconds);
    XCTAssertEqual(rotate_previous(timing_unit_kind::seconds), timing_unit_kind::minutes);
    XCTAssertEqual(rotate_previous(timing_unit_kind::minutes), timing_unit_kind::hours);
    XCTAssertEqual(rotate_previous(timing_unit_kind::hours), timing_unit_kind::fraction);
}

@end

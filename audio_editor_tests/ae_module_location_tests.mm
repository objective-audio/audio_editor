//
//  ae_module_location_tests.mm
//

#import <XCTest/XCTest.h>
#include <audio_editor_core/ae_module_location.h>

using namespace yas;
using namespace yas::ae;

@interface ae_module_location_tests : XCTestCase

@end

@implementation ae_module_location_tests

- (void)test_equal {
    identifier const id_1;
    identifier const id_2;
    proc::time::range const range_1(0, 1);
    proc::time::range const range_2(0, 2);
    uint32_t const sample_rate_1 = 1;
    uint32_t const sample_rate_2 = 2;

    XCTAssertTrue(module_location::make_value(id_1, range_1, sample_rate_1) ==
                  module_location::make_value(id_1, range_1, sample_rate_1));
    XCTAssertFalse(module_location::make_value(id_1, range_1, sample_rate_1) ==
                   module_location::make_value(id_2, range_1, sample_rate_1));
    XCTAssertFalse(module_location::make_value(id_1, range_1, sample_rate_1) ==
                   module_location::make_value(id_1, range_2, sample_rate_1));
    XCTAssertFalse(module_location::make_value(id_1, range_1, sample_rate_1) ==
                   module_location::make_value(id_1, range_1, sample_rate_2));

    XCTAssertFalse(module_location::make_value(id_1, range_1, sample_rate_1) !=
                   module_location::make_value(id_1, range_1, sample_rate_1));
    XCTAssertTrue(module_location::make_value(id_1, range_1, sample_rate_1) !=
                  module_location::make_value(id_2, range_1, sample_rate_1));
    XCTAssertTrue(module_location::make_value(id_1, range_1, sample_rate_1) !=
                  module_location::make_value(id_1, range_2, sample_rate_1));
    XCTAssertTrue(module_location::make_value(id_1, range_1, sample_rate_1) !=
                  module_location::make_value(id_1, range_1, sample_rate_2));
}

@end

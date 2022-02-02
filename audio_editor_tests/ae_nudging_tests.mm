//
//  ae_nudging_tests.mm
//

#import <XCTest/XCTest.h>
#include <audio_editor_core/ae_nudging.h>
#include "ae_nudging_test_utils.h"

using namespace yas;
using namespace yas::ae;

@interface ae_nudging_tests : XCTestCase

@end

@implementation ae_nudging_tests

- (void)test_initial {
    auto const timing = std::make_shared<test_utils::timing_stub>(48000);
    auto const nudging = nudging::make_shared(timing);

    XCTAssertEqual(nudging->unit_index(), 0);
    XCTAssertEqual(nudging->offset_count(), 1);
}

- (void)test_rotate_unit_index {
    auto const timing = std::make_shared<test_utils::timing_stub>(48000);
    auto const nudging = nudging::make_shared(timing);

    XCTAssertEqual(nudging->unit_index(), 0);

    nudging->rotate_unit_index();

    XCTAssertEqual(nudging->unit_index(), 3);

    nudging->rotate_unit_index();

    XCTAssertEqual(nudging->unit_index(), 2);

    nudging->rotate_unit_index();

    XCTAssertEqual(nudging->unit_index(), 1);
}

- (void)test_set_offset_count {
    auto const timing = std::make_shared<test_utils::timing_stub>(48000);
    auto const nudging = nudging::make_shared(timing);

    XCTAssertEqual(nudging->offset_count(), 1);

    nudging->set_offset_count(2);

    XCTAssertEqual(nudging->offset_count(), 2);
}

@end

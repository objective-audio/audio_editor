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
    auto const timing = std::make_shared<test_utils::timing_stub>(48000, 1);
    auto const nudging = nudging::make_shared(timing);

    XCTAssertEqual(nudging->kind(), nudging_kind::fraction);
    XCTAssertEqual(nudging->unit_count(), 1);
    XCTAssertEqual(nudging->unit_sample_count(), 1);
}

- (void)test_set_kind {
    auto const timing = std::make_shared<test_utils::timing_stub>(48000, 1);
    auto const nudging = nudging::make_shared(timing);

    XCTAssertEqual(nudging->kind(), nudging_kind::fraction);

    nudging->set_kind(nudging_kind::second);

    XCTAssertEqual(nudging->kind(), nudging_kind::second);

    nudging->set_kind(nudging_kind::minute);

    XCTAssertEqual(nudging->kind(), nudging_kind::minute);
}

- (void)test_set_unit_count {
    auto const timing = std::make_shared<test_utils::timing_stub>(48000, 1);
    auto const nudging = nudging::make_shared(timing);

    XCTAssertEqual(nudging->unit_count(), 1);

    nudging->set_unit_count(2);

    XCTAssertEqual(nudging->unit_count(), 2);
}

- (void)test_unit_sample_count {
    auto const timing = std::make_shared<test_utils::timing_stub>(48000, 1);
    auto const nudging = nudging::make_shared(timing);

    XCTAssertEqual(nudging->unit_sample_count(), 1);

    nudging->set_kind(nudging_kind::second);

    XCTAssertEqual(nudging->unit_sample_count(), 48000);

    nudging->set_kind(nudging_kind::minute);

    XCTAssertEqual(nudging->unit_sample_count(), 48000 * 60);

    nudging->set_unit_count(2);

    nudging->set_kind(nudging_kind::fraction);

    XCTAssertEqual(nudging->unit_sample_count(), 2);

    nudging->set_kind(nudging_kind::second);

    XCTAssertEqual(nudging->unit_sample_count(), 48000 * 2);

    nudging->set_kind(nudging_kind::minute);

    XCTAssertEqual(nudging->unit_sample_count(), 48000 * 60 * 2);
}

@end

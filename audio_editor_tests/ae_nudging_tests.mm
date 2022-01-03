//
//  ae_nudging_tests.mm
//

#import <XCTest/XCTest.h>
#include <audio_editor_core/ae_nudging.h>

using namespace yas;
using namespace yas::ae;

@interface ae_nudging_tests : XCTestCase

@end

@implementation ae_nudging_tests

- (void)test_initial {
    auto const nudging = nudging::make_shared(48000);

    XCTAssertEqual(nudging->kind(), nudging_kind::sample);
    XCTAssertEqual(nudging->unit_count(), 1);
    XCTAssertEqual(nudging->unit_sample_count(), 1);
}

- (void)test_set_kind {
    auto const nudging = nudging::make_shared(48000);

    XCTAssertEqual(nudging->kind(), nudging_kind::sample);

    nudging->set_kind(nudging_kind::second);

    XCTAssertEqual(nudging->kind(), nudging_kind::second);
}

- (void)test_set_unit_count {
    auto const nudging = nudging::make_shared(48000);

    XCTAssertEqual(nudging->unit_count(), 1);

    nudging->set_unit_count(2);

    XCTAssertEqual(nudging->unit_count(), 2);
}

- (void)test_unit_sample_count {
    auto const nudging = nudging::make_shared(48000);

    XCTAssertEqual(nudging->unit_sample_count(), 1);

    nudging->set_kind(nudging_kind::second);

    XCTAssertEqual(nudging->unit_sample_count(), 48000);

    nudging->set_unit_count(2);

    XCTAssertEqual(nudging->unit_sample_count(), 96000);
}

@end

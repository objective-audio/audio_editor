//
//  ae_module_index_tests.mm
//

#import <XCTest/XCTest.h>
#include <audio_editor_core/ae_module_index.hpp>

using namespace yas;
using namespace yas::ae;

@interface ae_module_index_tests : XCTestCase

@end

@implementation ae_module_index_tests

- (void)test_equal {
    auto const [object_id_1, object_id_2] = [self make_object_ids];
    time::range const range_1{0, 1};
    time::range const range_2{1, 1};
    track_index_t const track_1 = 1;
    track_index_t const track_2 = 2;

    module_index const index_id1_r1_t1{object_id_1, range_1, track_1};
    module_index const index_id2_r1_t1{object_id_2, range_1, track_1};
    module_index const index_id1_r2_t1{object_id_1, range_2, track_1};
    module_index const index_id1_r1_t2{object_id_1, range_1, track_2};

    // object_id以外は判定には使われないので等しい
    XCTAssertTrue(index_id1_r1_t1 == index_id1_r2_t1);
    XCTAssertTrue(index_id1_r1_t1 == index_id1_r1_t2);
    XCTAssertFalse(index_id1_r1_t1 != index_id1_r2_t1);
    XCTAssertFalse(index_id1_r1_t1 != index_id1_r1_t2);
    // object_idだけが違っていたら等しくない
    XCTAssertFalse(index_id1_r1_t1 == index_id2_r1_t1);
    XCTAssertTrue(index_id1_r1_t1 != index_id2_r1_t1);
}

- (void)test_less_than {
    auto const [object_id_1, object_id_2] = [self make_object_ids];
    time::range const range_1{0, 1};
    time::range const range_2{1, 1};
    track_index_t const track_1 = 1;
    track_index_t const track_2 = 2;

    module_index const index_id1_r1_t1{object_id_1, range_1, track_1};
    module_index const index_id2_r1_t1{object_id_2, range_1, track_1};
    module_index const index_id1_r2_t1{object_id_1, range_2, track_1};
    module_index const index_id1_r1_t2{object_id_1, range_1, track_2};

    XCTAssertTrue(index_id1_r1_t1 < index_id2_r1_t1);
    XCTAssertTrue(index_id1_r1_t1 < index_id1_r2_t1);
    XCTAssertTrue(index_id1_r1_t1 < index_id1_r1_t2);
    XCTAssertTrue(index_id1_r1_t2 < index_id1_r2_t1);
}

- (std::pair<db::object_id, db::object_id>)make_object_ids {
    auto const object_id_1 = db::make_temporary_id();
    auto const object_id_2 = db::make_temporary_id();

    if (object_id_1.identifier() < object_id_2.identifier()) {
        return std::make_pair(object_id_1, object_id_2);
    } else {
        return std::make_pair(object_id_2, object_id_1);
    }
}

@end

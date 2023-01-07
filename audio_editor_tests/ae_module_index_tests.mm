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
    auto const object_id_1 = db::make_temporary_id();
    auto const object_id_2 = db::make_temporary_id();

    module_index const index1_1{object_id_1, time::range{0, 1}};
    module_index const index2_1{object_id_2, time::range{0, 1}};
    module_index const index1_2{object_id_1, time::range{0, 2}};

    XCTAssertTrue(index1_1 == index1_2);
    XCTAssertFalse(index1_1 != index1_2);
    XCTAssertFalse(index1_1 == index2_1);
    XCTAssertTrue(index1_1 != index2_1);
}

- (void)test_less_than {
    auto const object_id_1 = db::make_temporary_id();
    auto const object_id_2 = db::make_temporary_id();

    module_index const index1_1{object_id_1, time::range{0, 1}};
    module_index const index2_1{object_id_2, time::range{0, 1}};
    module_index const index1_2{object_id_1, time::range{0, 2}};

    XCTAssertTrue(index1_1 < index1_2);
    XCTAssertEqual(index1_1 < index2_1, object_id_1.identifier() < object_id_2.identifier());
}

@end

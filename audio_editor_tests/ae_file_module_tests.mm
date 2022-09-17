//
//  ae_file_module_tests.mm
//

#import <XCTest/XCTest.h>
#import <audio_editor_core/ae_file_module.h>

using namespace yas;
using namespace yas::ae;

@interface ae_file_module_tests : XCTestCase

@end

@implementation ae_file_module_tests

- (void)test_head_dropped {
    file_module const module{db::make_temporary_id(), "head_dropped_name", {1, 3}, 10, "test-name-1"};

    XCTAssertEqual(module.head_dropped(1), std::nullopt);
    XCTAssertEqual(module.head_dropped(2).value().name, "head_dropped_name");
    XCTAssertEqual(module.head_dropped(2).value().range, (time::range{2, 2}));
    XCTAssertEqual(module.head_dropped(2).value().file_frame, 11);
    XCTAssertEqual(module.head_dropped(2).value().file_name, "test-name-1");
    XCTAssertEqual(module.head_dropped(3).value().name, "head_dropped_name");
    XCTAssertEqual(module.head_dropped(3).value().range, (time::range{3, 1}));
    XCTAssertEqual(module.head_dropped(3).value().file_frame, 12);
    XCTAssertEqual(module.head_dropped(3).value().file_name, "test-name-1");
    XCTAssertEqual(module.head_dropped(4), std::nullopt);
}

- (void)test_tail_dropped {
    file_module const module{db::make_temporary_id(), "tail_dropped_name", {1, 3}, 10, "test-name-2"};

    XCTAssertEqual(module.tail_dropped(1), std::nullopt);
    XCTAssertEqual(module.tail_dropped(2).value().name, "tail_dropped_name");
    XCTAssertEqual(module.tail_dropped(2).value().range, (time::range{1, 1}));
    XCTAssertEqual(module.tail_dropped(2).value().file_frame, 10);
    XCTAssertEqual(module.tail_dropped(2).value().file_name, "test-name-2");
    XCTAssertEqual(module.tail_dropped(3).value().name, "tail_dropped_name");
    XCTAssertEqual(module.tail_dropped(3).value().range, (time::range{1, 2}));
    XCTAssertEqual(module.tail_dropped(3).value().file_frame, 10);
    XCTAssertEqual(module.tail_dropped(3).value().file_name, "test-name-2");
    XCTAssertEqual(module.tail_dropped(4), std::nullopt);
}

- (void)test_offset {
    file_module const module{db::make_temporary_id(), "offset_name", {1, 3}, 10, "test-name-3"};

    XCTAssertEqual(module.offset(-1).name, "offset_name");
    XCTAssertEqual(module.offset(-1).range, (time::range{0, 3}));
    XCTAssertEqual(module.offset(-1).file_frame, 10);
    XCTAssertEqual(module.offset(-1).file_name, "test-name-3");
    XCTAssertEqual(module.offset(0).name, "offset_name");
    XCTAssertEqual(module.offset(0).range, (time::range{1, 3}));
    XCTAssertEqual(module.offset(0).file_frame, 10);
    XCTAssertEqual(module.offset(0).file_name, "test-name-3");
    XCTAssertEqual(module.offset(1).name, "offset_name");
    XCTAssertEqual(module.offset(1).range, (time::range{2, 3}));
    XCTAssertEqual(module.offset(1).file_frame, 10);
    XCTAssertEqual(module.offset(1).file_name, "test-name-3");
}

@end

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
    file_module const module{.name = "head_dropped_name", .range = {1, 3}, .file_frame = 10};

    XCTAssertEqual(module.head_dropped(1), std::nullopt);
    XCTAssertTrue(module.head_dropped(2).value().is_equal_location(file_module{.range = {2, 2}, .file_frame = 11}));
    XCTAssertEqual(module.head_dropped(2).value().name, "head_dropped_name");
    XCTAssertTrue(module.head_dropped(3).value().is_equal_location(file_module{.range = {3, 1}, .file_frame = 12}));
    XCTAssertEqual(module.head_dropped(3).value().name, "head_dropped_name");
    XCTAssertEqual(module.head_dropped(4), std::nullopt);
}

- (void)test_tail_dropped {
    file_module const module{.name = "tail_dropped_name", .range = {1, 3}, .file_frame = 10};

    XCTAssertEqual(module.tail_dropped(1), std::nullopt);
    XCTAssertTrue(module.tail_dropped(2).value().is_equal_location(file_module{.range = {1, 1}, .file_frame = 10}));
    XCTAssertEqual(module.tail_dropped(2).value().name, "tail_dropped_name");
    XCTAssertTrue(module.tail_dropped(3).value().is_equal_location(file_module{.range = {1, 2}, .file_frame = 10}));
    XCTAssertEqual(module.tail_dropped(3).value().name, "tail_dropped_name");
    XCTAssertEqual(module.tail_dropped(4), std::nullopt);
}

- (void)test_offset {
    file_module const module{.name = "offset_name", .range = {1, 3}, .file_frame = 10};

    XCTAssertTrue(module.offset(-1).is_equal_location(file_module{.range = {0, 3}, .file_frame = 10}));
    XCTAssertEqual(module.offset(-1).name, "offset_name");
    XCTAssertTrue(module.offset(0).is_equal_location(file_module{.range = {1, 3}, .file_frame = 10}));
    XCTAssertEqual(module.offset(0).name, "offset_name");
    XCTAssertTrue(module.offset(1).is_equal_location(file_module{.range = {2, 3}, .file_frame = 10}));
    XCTAssertEqual(module.offset(1).name, "offset_name");
}

@end

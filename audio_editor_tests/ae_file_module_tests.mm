//
//  ae_file_module_tests.mm
//

#import <XCTest/XCTest.h>
#import <audio_editor_core/audio_editor_core_umbrella.h>

using namespace yas;
using namespace yas::ae;

@interface ae_file_module_tests : XCTestCase

@end

@implementation ae_file_module_tests

- (void)test_head_dropped {
    file_module const module{.range = {1, 3}, .file_frame = 10};

    XCTAssertEqual(module.head_dropped(1), std::nullopt);
    XCTAssertEqual(module.head_dropped(2), (file_module{.range = {2, 2}, .file_frame = 11}));
    XCTAssertEqual(module.head_dropped(3), (file_module{.range = {3, 1}, .file_frame = 12}));
    XCTAssertEqual(module.head_dropped(4), std::nullopt);
}

- (void)test_tail_dropped {
    file_module const module{.range = {1, 3}, .file_frame = 10};

    XCTAssertEqual(module.tail_dropped(1), std::nullopt);
    XCTAssertEqual(module.tail_dropped(2), (file_module{.range = {1, 1}, .file_frame = 10}));
    XCTAssertEqual(module.tail_dropped(3), (file_module{.range = {1, 2}, .file_frame = 10}));
    XCTAssertEqual(module.tail_dropped(4), std::nullopt);
}

- (void)test_offset {
    file_module const module{.range = {1, 3}, .file_frame = 10};

    XCTAssertEqual(module.offset(-1), (file_module{.range = {0, 3}, .file_frame = 10}));
    XCTAssertEqual(module.offset(0), (file_module{.range = {1, 3}, .file_frame = 10}));
    XCTAssertEqual(module.offset(1), (file_module{.range = {2, 3}, .file_frame = 10}));
}

@end

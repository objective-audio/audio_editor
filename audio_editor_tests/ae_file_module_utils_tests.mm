//
//  ae_file_module_utils_tests.mm
//

#import <XCTest/XCTest.h>
#import <audio_editor_core/ae_file_module_utils.h>

using namespace yas;
using namespace yas::ae;

@interface ae_file_module_utils_tests : XCTestCase

@end

@implementation ae_file_module_utils_tests

- (void)test_can_split_time_range {
    time::range const range{1, 3};

    XCTAssertFalse(file_module_utils::can_split_time_range(range, 0));
    XCTAssertFalse(file_module_utils::can_split_time_range(range, 1));
    XCTAssertTrue(file_module_utils::can_split_time_range(range, 2));
    XCTAssertTrue(file_module_utils::can_split_time_range(range, 3));
    XCTAssertFalse(file_module_utils::can_split_time_range(range, 4));
}

- (void)test_overlapped_modules {
    file_track_module_map_t const modules{{time::range{0, 3}, {.range = time::range{0, 3}, .file_frame = 0}},
                                          {time::range{3, 2}, {.range = time::range{3, 2}, .file_frame = 3}}};

    {
        auto const overlapped_modules = file_module_utils::overlapped_modules(modules, {-2, 2});
        XCTAssertEqual(overlapped_modules.size(), 0);
    }

    {
        auto const overlapped_modules = file_module_utils::overlapped_modules(modules, {-1, 2});
        XCTAssertEqual(overlapped_modules.size(), 1);
        XCTAssertEqual(overlapped_modules.at(0).range, (time::range{0, 3}));
    }

    {
        auto const overlapped_modules = file_module_utils::overlapped_modules(modules, {0, 2});
        XCTAssertEqual(overlapped_modules.size(), 1);
        XCTAssertEqual(overlapped_modules.at(0).range, (time::range{0, 3}));
    }

    {
        auto const overlapped_modules = file_module_utils::overlapped_modules(modules, {1, 2});
        XCTAssertEqual(overlapped_modules.size(), 1);
        XCTAssertEqual(overlapped_modules.at(0).range, (time::range{0, 3}));
    }

    {
        auto const overlapped_modules = file_module_utils::overlapped_modules(modules, {2, 2});
        XCTAssertEqual(overlapped_modules.size(), 2);
        XCTAssertEqual(overlapped_modules.at(0).range, (time::range{0, 3}));
        XCTAssertEqual(overlapped_modules.at(1).range, (time::range{3, 2}));
    }

    {
        auto const overlapped_modules = file_module_utils::overlapped_modules(modules, {3, 2});
        XCTAssertEqual(overlapped_modules.size(), 1);
        XCTAssertEqual(overlapped_modules.at(0).range, (time::range{3, 2}));
    }

    {
        auto const overlapped_modules = file_module_utils::overlapped_modules(modules, {4, 2});
        XCTAssertEqual(overlapped_modules.size(), 1);
        XCTAssertEqual(overlapped_modules.at(0).range, (time::range{3, 2}));
    }

    {
        auto const overlapped_modules = file_module_utils::overlapped_modules(modules, {5, 2});
        XCTAssertEqual(overlapped_modules.size(), 0);
    }
}

@end

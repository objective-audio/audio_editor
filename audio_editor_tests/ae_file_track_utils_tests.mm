//
//  ae_file_track_utils_tests.mm
//

#import <XCTest/XCTest.h>
#import <audio_editor_core/ae_file_track_utils.h>

using namespace yas;
using namespace yas::ae;

@interface ae_file_track_utils_tests : XCTestCase

@end

@implementation ae_file_track_utils_tests

- (void)test_overlapped_modules {
    file_module_object const module1{db::make_temporary_id(), {"", time::range{0, 3}, 0, ""}};
    file_module_object const module2{db::make_temporary_id(), {"", time::range{3, 2}, 3, ""}};

    file_track_module_map_t const modules{{module1.index(), module1}, {module2.index(), module2}};

    {
        auto const overlapped_modules = file_track_utils::overlapped_modules(modules, {-2, 2});
        XCTAssertEqual(overlapped_modules.size(), 0);
    }

    {
        auto const overlapped_modules = file_track_utils::overlapped_modules(modules, {-1, 2});
        XCTAssertEqual(overlapped_modules.size(), 1);
        XCTAssertEqual(overlapped_modules.at(0).value.range, (time::range{0, 3}));
    }

    {
        auto const overlapped_modules = file_track_utils::overlapped_modules(modules, {0, 2});
        XCTAssertEqual(overlapped_modules.size(), 1);
        XCTAssertEqual(overlapped_modules.at(0).value.range, (time::range{0, 3}));
    }

    {
        auto const overlapped_modules = file_track_utils::overlapped_modules(modules, {1, 2});
        XCTAssertEqual(overlapped_modules.size(), 1);
        XCTAssertEqual(overlapped_modules.at(0).value.range, (time::range{0, 3}));
    }

    {
        auto const overlapped_modules = file_track_utils::overlapped_modules(modules, {2, 2});
        XCTAssertEqual(overlapped_modules.size(), 2);
        XCTAssertEqual(overlapped_modules.at(0).value.range, (time::range{0, 3}));
        XCTAssertEqual(overlapped_modules.at(1).value.range, (time::range{3, 2}));
    }

    {
        auto const overlapped_modules = file_track_utils::overlapped_modules(modules, {3, 2});
        XCTAssertEqual(overlapped_modules.size(), 1);
        XCTAssertEqual(overlapped_modules.at(0).value.range, (time::range{3, 2}));
    }

    {
        auto const overlapped_modules = file_track_utils::overlapped_modules(modules, {4, 2});
        XCTAssertEqual(overlapped_modules.size(), 1);
        XCTAssertEqual(overlapped_modules.at(0).value.range, (time::range{3, 2}));
    }

    {
        auto const overlapped_modules = file_track_utils::overlapped_modules(modules, {5, 2});
        XCTAssertEqual(overlapped_modules.size(), 0);
    }
}

@end

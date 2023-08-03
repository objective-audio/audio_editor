//
//  ae_module_pool_utils_tests.mm
//

#import <XCTest/XCTest.h>
#import <audio_editor_core/ae_module_pool_utils.h>

using namespace yas;
using namespace yas::ae;

@interface ae_module_pool_utils_tests : XCTestCase

@end

@implementation ae_module_pool_utils_tests

- (void)test_overlapped_modules {
    module_object const module1{db::make_temporary_id(), {"", time::range{0, 3}, 0, 0, ""}};
    module_object const module2{db::make_temporary_id(), {"", time::range{3, 2}, 0, 3, ""}};
    module_object const module3{db::make_temporary_id(), {"", time::range{5, 1}, 1, 5, ""}};

    module_pool_module_map_t const modules{
        {module1.index(), module1}, {module2.index(), module2}, {module3.index(), module3}};

    // track0での被り

    {
        auto const overlapped_modules = module_pool_utils::overlapped_modules(modules, 0, {-2, 2});
        XCTAssertEqual(overlapped_modules.size(), 0);
    }

    {
        auto const overlapped_modules = module_pool_utils::overlapped_modules(modules, 0, {-1, 2});
        XCTAssertEqual(overlapped_modules.size(), 1);
        XCTAssertEqual(overlapped_modules.at(0).value.range, (time::range{0, 3}));
    }

    {
        auto const overlapped_modules = module_pool_utils::overlapped_modules(modules, 0, {0, 2});
        XCTAssertEqual(overlapped_modules.size(), 1);
        XCTAssertEqual(overlapped_modules.at(0).value.range, (time::range{0, 3}));
    }

    {
        auto const overlapped_modules = module_pool_utils::overlapped_modules(modules, 0, {1, 2});
        XCTAssertEqual(overlapped_modules.size(), 1);
        XCTAssertEqual(overlapped_modules.at(0).value.range, (time::range{0, 3}));
    }

    {
        auto const overlapped_modules = module_pool_utils::overlapped_modules(modules, 0, {2, 2});
        XCTAssertEqual(overlapped_modules.size(), 2);
        XCTAssertEqual(overlapped_modules.at(0).value.range, (time::range{0, 3}));
        XCTAssertEqual(overlapped_modules.at(1).value.range, (time::range{3, 2}));
    }

    {
        auto const overlapped_modules = module_pool_utils::overlapped_modules(modules, 0, {3, 2});
        XCTAssertEqual(overlapped_modules.size(), 1);
        XCTAssertEqual(overlapped_modules.at(0).value.range, (time::range{3, 2}));
    }

    {
        auto const overlapped_modules = module_pool_utils::overlapped_modules(modules, 0, {4, 2});
        XCTAssertEqual(overlapped_modules.size(), 1);
        XCTAssertEqual(overlapped_modules.at(0).value.range, (time::range{3, 2}));
    }

    {
        auto const overlapped_modules = module_pool_utils::overlapped_modules(modules, 0, {5, 2});
        XCTAssertEqual(overlapped_modules.size(), 0);
    }

    // track1での被り

    {
        auto const overlapped_modules = module_pool_utils::overlapped_modules(modules, 1, {3, 2});
        XCTAssertEqual(overlapped_modules.size(), 0);
    }

    {
        auto const overlapped_modules = module_pool_utils::overlapped_modules(modules, 1, {4, 2});
        XCTAssertEqual(overlapped_modules.size(), 1);
    }

    {
        auto const overlapped_modules = module_pool_utils::overlapped_modules(modules, 1, {5, 2});
        XCTAssertEqual(overlapped_modules.size(), 1);
    }

    {
        auto const overlapped_modules = module_pool_utils::overlapped_modules(modules, 1, {6, 2});
        XCTAssertEqual(overlapped_modules.size(), 0);
    }
}

@end

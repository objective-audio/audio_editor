//
//  ae_pasting_file_module_tests.mm
//

#import <XCTest/XCTest.h>
#include <audio_editor_core/ae_pasteboard_types.h>
#include <sstream>

using namespace yas;
using namespace yas::ae;

@interface ae_pasting_file_module_tests : XCTestCase

@end

@implementation ae_pasting_file_module_tests

- (void)test_data {
    pasting_file_module const module{.file_frame = 1, .length = 2, .file_name = "test-name-1"};

    XCTAssertEqual(module.data(), "file_frame=1,file_name=test-name-1,kind=file_module,length=2");
}

- (void)test_make {
    auto const module =
        pasting_file_module::make_value("file_frame=10,file_name=test-name-2,kind=file_module,length=20");

    XCTAssertTrue(module.has_value());
    XCTAssertEqual(module.value().file_frame, 10);
    XCTAssertEqual(module.value().length, 20);
    XCTAssertEqual(module.value().file_name, "test-name-2");

    XCTAssertFalse(pasting_file_module::make_value("").has_value());
}

- (void)test_to_string {
    pasting_file_module const module{.file_frame = 100, .length = 200, .file_name = "test-name-3"};

    XCTAssertEqual(to_string(module), "{file_frame:100, length:200, file_name:test-name-3}");
}

- (void)test_ostream {
    pasting_file_module const value{.file_frame = 1000, .length = 2000, .file_name = "test-name-4"};

    std::ostringstream stream;
    stream << value;
    XCTAssertEqual(stream.str(), to_string(value));
}

@end

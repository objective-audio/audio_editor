//
//  ae_pasting_file_module_tests.mm
//

#import <XCTest/XCTest.h>
#include <audio_editor_core/ae_pasting_file_module.hpp>
#include <sstream>

using namespace yas;
using namespace yas::ae;

@interface ae_pasting_file_module_tests : XCTestCase

@end

@implementation ae_pasting_file_module_tests

- (void)test_to_string {
    pasting_file_module const module{"test-name-3", 100, {200, 300}, "test-file-name-3"};

    XCTAssertEqual(to_string(module),
                   "{name:test-name-3, file_frame:100, range:{200, 300}, file_name:test-file-name-3}");
}

- (void)test_ostream {
    pasting_file_module const value{"test-name-4", 1000, {2000, 3000}, "test-file-name-4"};

    std::ostringstream stream;
    stream << value;
    XCTAssertEqual(stream.str(), to_string(value));
}

@end

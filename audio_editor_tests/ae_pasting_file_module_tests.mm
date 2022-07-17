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
    pasting_file_module const module{
        .name = "test-name-1", .file_frame = -1, .range = {2, 3}, .file_name = "test-file-name-1"};

    XCTAssertEqual(module.json_string(),
                   "{\"frame\":2,\"kind\":\"file_module\",\"length\":3,\"file_name\":\"test-file-name-1\","
                   "\"name\":\"test-name-1\",\"file_frame\":-1}");
}

- (void)test_make {
    auto const module =
        pasting_file_module::make_value("{\"file_frame\":-10,\"file_name\":\"test-file-name-2\",\"kind\":\"file_"
                                        "module\",\"frame\":20,\"length\":30,\"name\":\"test-name-2\"}");

    XCTAssertTrue(module.has_value());
    XCTAssertEqual(module.value().name, "test-name-2");
    XCTAssertEqual(module.value().file_frame, -10);
    XCTAssertEqual(module.value().range, time::range(20, 30));
    XCTAssertEqual(module.value().file_name, "test-file-name-2");

    XCTAssertFalse(pasting_file_module::make_value("").has_value());
}

- (void)test_to_string {
    pasting_file_module const module{
        .name = "test-name-3", .file_frame = 100, .range = {200, 300}, .file_name = "test-file-name-3"};

    XCTAssertEqual(to_string(module),
                   "{name:test-name-3, file_frame:100, range:{200, 300}, file_name:test-file-name-3}");
}

- (void)test_ostream {
    pasting_file_module const value{
        .name = "test-name-4", .file_frame = 1000, .range = {2000, 3000}, .file_name = "test-file-name-4"};

    std::ostringstream stream;
    stream << value;
    XCTAssertEqual(stream.str(), to_string(value));
}

@end

//
//  file_info_loader_tests.mm
//

#import <XCTest/XCTest.h>
#import <ae-core/app/features/file_info_loader.h>
#include <audio-engine/umbrella.hpp>
#include "utils/test_utils.h"

using namespace yas;
using namespace yas::ae;

@interface file_info_loader_tests : XCTestCase

@end

@implementation file_info_loader_tests

- (void)setUp {
    [super setUp];
    test_utils::remove_contents_in_test_directory();
    test_utils::create_test_directory();
}

- (void)tearDown {
    test_utils::remove_contents_in_test_directory();
    [super tearDown];
}

- (void)test_load_file_info {
    auto const path = test_utils::test_path().append("src.wav");
    auto const file_result = audio::file::make_created({.file_path = path,
                                                        .pcm_format = audio::pcm_format::int16,
                                                        .settings = audio::wave_file_settings(48000, 1, 16)});
    XCTAssertTrue(file_result.is_success());
    auto const &file = file_result.value();

    auto buffer = audio::pcm_buffer(file->processing_format(), 4);
    auto *data = buffer.data_ptr_at_index<int16_t>(0);
    data[0] = 10;
    data[1] = 11;
    data[2] = 12;
    data[3] = 13;

    file->write_from_buffer(buffer);

    file->close();

    auto const loader = std::make_shared<file_info_loader>();
    auto const file_info = loader->load_file_info(path);

    XCTAssertEqual(file_info.value().sample_rate, 48000);
    XCTAssertEqual(file_info.value().length, 4);
}

@end

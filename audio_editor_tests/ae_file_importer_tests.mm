//
//  ae_file_importer_tests.mm
//

#import <XCTest/XCTest.h>
#include <audio/yas_audio_umbrella.h>
#import <audio_editor_core/ae_file_importer.h>
#include "ae_test_utils.h"

using namespace yas;
using namespace yas::ae;

namespace yas::ae::test_utils::file_importer {
uint32_t const sample_rate = 48000.0;
uint32_t const ch_count = 2;
audio::pcm_format const pcm_format = audio::pcm_format::int16;
}

@interface ae_file_importer_tests : XCTestCase

@end

@implementation ae_file_importer_tests

- (void)setUp {
    [super setUp];
    test_utils::remove_contents_in_test_directory();
    test_utils::create_test_directory();
}

- (void)tearDown {
    test_utils::remove_contents_in_test_directory();
    [super tearDown];
}

- (void)test_import {
    double const sample_rate = test_utils::file_importer::sample_rate;
    uint32_t const ch_count = test_utils::file_importer::ch_count;
    audio::pcm_format const pcm_format = test_utils::file_importer::pcm_format;
    project_format const project_format{.sample_rate = test_utils::file_importer::sample_rate,
                                        .channel_count = test_utils::file_importer::ch_count};

    auto const src_path = test_utils::test_path().append("src.wav");
    auto const dst_path = test_utils::test_path().append("dst.caf");

    [self setup_src_file:src_path];

    auto const worker = worker_stub::make_shared();
    auto const importer = file_importer::make_shared(worker, 0);

    worker->start();

    std::vector<bool> results;

    auto expectation = [self expectationWithDescription:@""];

    importer->import({.src_path = src_path,
                      .dst_path = dst_path,
                      .project_format = project_format,
                      .completion = [&results, &expectation](bool const result) {
                          results.emplace_back(result);
                          [expectation fulfill];
                      }});

    worker->process();

    [self waitForExpectations:@[expectation] timeout:10.0];

    XCTAssertEqual(results.size(), 1);
    XCTAssertTrue(results.at(0));

    auto const dst_file_result =
        audio::file::make_opened({.file_path = dst_path, .pcm_format = audio::pcm_format::int16});
    if (dst_file_result.is_error()) {
        XCTFail();
        return;
    }
    auto const dst_file = dst_file_result.value();

    auto const dst_file_format = dst_file->file_format();
    XCTAssertEqual(dst_file_format.sample_rate(), sample_rate);
    XCTAssertEqual(dst_file_format.channel_count(), ch_count);
    XCTAssertEqual(dst_file_format.pcm_format(), audio::pcm_format::float32);

    audio::format const dst_proc_format{
        {.sample_rate = sample_rate, .channel_count = ch_count, .pcm_format = pcm_format}};

    dst_file->set_processing_format(dst_proc_format);

    audio::pcm_buffer dst_buffer{dst_proc_format, 5};

    auto const read_result = dst_file->read_into_buffer(dst_buffer);

    if (read_result.is_error()) {
        XCTFail();
        return;
    }

    XCTAssertEqual(dst_buffer.frame_length(), 4);
    auto const dst_data_0 = dst_buffer.data_ptr_at_index<int16_t>(0);
    XCTAssertEqual(dst_data_0[0], 10);
    XCTAssertEqual(dst_data_0[1], 11);
    XCTAssertEqual(dst_data_0[2], 12);
    XCTAssertEqual(dst_data_0[3], 13);
    auto const dst_data_1 = dst_buffer.data_ptr_at_index<int16_t>(1);
    XCTAssertEqual(dst_data_1[0], 20);
    XCTAssertEqual(dst_data_1[1], 21);
    XCTAssertEqual(dst_data_1[2], 22);
    XCTAssertEqual(dst_data_1[3], 23);
}

- (bool)setup_src_file:(std::filesystem::path const &)src_path {
    double const sample_rate = test_utils::file_importer::sample_rate;
    uint32_t const ch_count = test_utils::file_importer::ch_count;
    audio::pcm_format const pcm_format = test_utils::file_importer::pcm_format;

    auto const wave_settings = audio::wave_file_settings(sample_rate, ch_count, 16);
    auto const src_file_result =
        audio::file::make_created({.file_path = src_path, .pcm_format = pcm_format, .settings = wave_settings});
    if (src_file_result.is_error()) {
        XCTFail();
        return false;
    }
    auto const src_file = src_file_result.value();

    audio::format const src_proc_format = src_file->processing_format();
    audio::pcm_buffer src_buffer{src_proc_format, 4};

    auto src_data_0 = src_buffer.data_ptr_at_index<int16_t>(0);
    src_data_0[0] = 10;
    src_data_0[1] = 11;
    src_data_0[2] = 12;
    src_data_0[3] = 13;
    auto src_data_1 = src_buffer.data_ptr_at_index<int16_t>(1);
    src_data_1[0] = 20;
    src_data_1[1] = 21;
    src_data_1[2] = 22;
    src_data_1[3] = 23;

    auto const write_result = src_file->write_from_buffer(src_buffer);
    src_file->close();

    if (write_result.is_error()) {
        XCTFail();
        return false;
    }

    return true;
}

@end

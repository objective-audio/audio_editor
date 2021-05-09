//
//  ae_file_track_tests.mm
//

#import <XCTest/XCTest.h>
#import <audio_editor_core/audio_editor_core_umbrella.h>
#import <processing/yas_processing_umbrella.h>

using namespace yas;
using namespace yas::ae;

@interface ae_file_track_tests : XCTestCase

@end

@implementation ae_file_track_tests

- (void)test_insert_module {
    auto const track = file_track::make_shared();

    XCTAssertEqual(track->modules().size(), 0);

    file_module const module1{.range = proc::time::range{0, 4}, .file_frame = 0};

    track->insert_module(module1);

    XCTAssertEqual(track->modules().size(), 1);
    XCTAssertEqual(track->modules().at(module1.range), module1);

    file_module const module2{.range = proc::time::range{4, 3}, .file_frame = 4};

    track->insert_module(module2);

    XCTAssertEqual(track->modules().size(), 2);
    XCTAssertEqual(track->modules().at(module2.range), module2);

    file_module const module3{.range = proc::time::range{-2, 2}, .file_frame = 7};

    track->insert_module(module3);

    XCTAssertEqual(track->modules().size(), 3);
    XCTAssertEqual(track->modules().at(module3.range), module3);

    auto iterator = track->modules().begin();
    XCTAssertEqual(iterator->first, module3.range);
    ++iterator;
    XCTAssertEqual(iterator->first, module1.range);
    ++iterator;
    XCTAssertEqual(iterator->first, module2.range);
}

@end

//
//  ae_id_generator_tests.mm
//

#import <XCTest/XCTest.h>
#import <audio_editor_core/audio_editor_core_umbrella.h>
#include <cpp_utils/yas_stl_utils.h>

using namespace yas;
using namespace yas::ae;

@interface ae_id_generator_tests : XCTestCase

@end

@implementation ae_id_generator_tests

- (void)test_generate {
    auto const generator = id_generator::make_shared();

    std::string const uuid = generator->generate();

    XCTAssertEqual(uuid.size(), 36);

    auto const splited = split(uuid, '-');
    XCTAssertEqual(splited.size(), 5);
    XCTAssertEqual(splited.at(0).size(), 8);
    XCTAssertEqual(splited.at(1).size(), 4);
    XCTAssertEqual(splited.at(2).size(), 4);
    XCTAssertEqual(splited.at(3).size(), 4);
    XCTAssertEqual(splited.at(4).size(), 12);
}

@end

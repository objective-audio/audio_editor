//
//  ae_uuid_generator_tests.mm
//

#import <XCTest/XCTest.h>
#import <ae-core/app/features/ae_uuid_generator.h>
#include <cpp-utils/stl_utils.h>

using namespace yas;
using namespace yas::ae;

@interface ae_uuid_generator_tests : XCTestCase

@end

@implementation ae_uuid_generator_tests

- (void)test_generate {
    auto const generator = std::make_shared<uuid_generator>();

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

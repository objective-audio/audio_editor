//
//  ae_edge_tests.mm
//

#import <XCTest/XCTest.h>
#include <audio_editor_core/ae_edge.h>
#include <sstream>

using namespace yas;
using namespace yas::ae;

@interface ae_edge_tests : XCTestCase

@end

@implementation ae_edge_tests

- (void)test_equal {
    edge const value1a{0, 1};
    edge const value1b{0, 1};
    edge const value2{2, 1};
    edge const value3{0, 3};

    XCTAssertTrue(value1a == value1a);
    XCTAssertTrue(value1a == value1b);
    XCTAssertFalse(value1a == value2);
    XCTAssertFalse(value1a == value3);

    XCTAssertFalse(value1a != value1a);
    XCTAssertFalse(value1a != value1b);
    XCTAssertTrue(value1a != value2);
    XCTAssertTrue(value1a != value3);
}

- (void)test_range {
    XCTAssertEqual((edge{0, 1}.range()), (time::range{0, 1}));
    XCTAssertEqual((edge{1, 3}.range()), (time::range{1, 2}));

    XCTAssertFalse((edge{2, 2}.range().has_value()));
    XCTAssertFalse((edge{2, 1}.range().has_value()));
}

- (void)test_zero {
    auto const zero = edge::zero();

    XCTAssertEqual(zero.begin_frame, 0);
    XCTAssertEqual(zero.end_frame, 0);
}

- (void)test_to_string {
    edge const value{100, 200};

    XCTAssertEqual(to_string(value), "{100, 200}");
}

- (void)test_ostream {
    edge const value{100, 200};

    std::ostringstream stream;
    stream << value;
    XCTAssertEqual(stream.str(), to_string(value));
}

@end

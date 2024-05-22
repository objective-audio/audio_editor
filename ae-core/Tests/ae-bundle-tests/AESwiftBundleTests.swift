import XCTest

@testable import ae_swift_bundle

final class AESwiftBundleTests: XCTestCase {
    func testColor() throws {
        XCTAssertNoThrow(try AESwiftBundle.color(name: "background"))
        XCTAssertThrowsError(try AESwiftBundle.color(name: "xxx"))
    }
}

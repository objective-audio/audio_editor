//
//  file_importer_resource_tests.mm
//

#import <XCTest/XCTest.h>
#import <ae-core/app/features/file_importer_resource.h>

using namespace yas;
using namespace yas::ae;

@interface file_importer_resource_tests : XCTestCase

@end

@implementation file_importer_resource_tests

- (void)test_push_and_pull_context {
    auto const resource = std::make_shared<ae::file_importer_resource>();

    XCTAssertFalse(resource->pull_context_on_task().has_value());

    resource->push_context_on_main(
        {.src_path = std::filesystem::path("test/src0.wav"), .dst_path = std::filesystem::path("test/dst0.caf")});
    resource->push_context_on_main(
        {.src_path = std::filesystem::path("test/src1.wav"), .dst_path = std::filesystem::path("test/dst1.caf")});

    auto const context0 = resource->pull_context_on_task();

    XCTAssertTrue(context0.has_value());
    XCTAssertEqual(context0.value().src_path, std::filesystem::path("test/src0.wav"));
    XCTAssertEqual(context0.value().dst_path, std::filesystem::path("test/dst0.caf"));

    auto const context1 = resource->pull_context_on_task();

    XCTAssertTrue(context1.has_value());
    XCTAssertEqual(context1.value().src_path, std::filesystem::path("test/src1.wav"));
    XCTAssertEqual(context1.value().dst_path, std::filesystem::path("test/dst1.caf"));

    XCTAssertFalse(resource->pull_context_on_task().has_value());
}

- (void)test_cancel_context {
    auto const resource = std::make_shared<ae::file_importer_resource>();

    resource->push_context_on_main({.project_id = "0",
                                    .src_path = std::filesystem::path("test/src0.wav"),
                                    .dst_path = std::filesystem::path("test/dst0.caf")});
    resource->push_context_on_main({.project_id = "1",
                                    .src_path = std::filesystem::path("test/src1.wav"),
                                    .dst_path = std::filesystem::path("test/dst1.caf")});

    resource->cancel_on_main({"0"});

    auto const context1 = resource->pull_context_on_task();

    XCTAssertTrue(context1.has_value());
    XCTAssertEqual(context1.value().src_path, std::filesystem::path("test/src1.wav"));
    XCTAssertEqual(context1.value().dst_path, std::filesystem::path("test/dst1.caf"));
}

- (void)test_pull_cancel_ids {
    auto const resource = std::make_shared<ae::file_importer_resource>();

    resource->cancel_on_main({"2"});
    resource->cancel_on_main({"3"});

    auto const ids = resource->pull_cancel_ids();

    XCTAssertEqual(ids.size(), 2);
    XCTAssertEqual(ids.at(0), "2");
    XCTAssertEqual(ids.at(1), "3");

    XCTAssertTrue(resource->pull_cancel_ids().empty());
}

- (void)test_clear_cancel_ids_at_pull_context {
    auto const resource = std::make_shared<ae::file_importer_resource>();

    resource->cancel_on_main({"4"});
    resource->cancel_on_main({"5"});

    auto const context = resource->pull_context_on_task();

    XCTAssertTrue(resource->pull_cancel_ids().empty());
}

@end

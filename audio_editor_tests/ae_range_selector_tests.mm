//
//  ae_range_selector_tests.mm
//

#import <XCTest/XCTest.h>
#include <audio_editor_core/ae_range_selector.hpp>

using namespace yas;
using namespace yas::ae;

namespace yas::ae::test_utils {
struct player_stub : player_for_range_selector {
    void set_is_playing(bool const value) {
        this->_is_playing_holder->set_value(value);
    }

    [[nodiscard]] bool is_playing() const override {
        return this->_is_playing_holder->value();
    }

    [[nodiscard]] observing::syncable observe_is_playing(std::function<void(bool const &)> &&handler) override {
        return this->_is_playing_holder->observe(std::move(handler));
    }

   private:
    observing::value::holder_ptr<bool> const _is_playing_holder = observing::value::holder<bool>::make_shared(false);
};

struct deselector_stub : deselector_for_range_selector {
    std::size_t deselect_all_called_count = 0;

    void deselect_all() override {
        ++this->deselect_all_called_count;
    }
};
}

@interface ae_range_selector_tests : XCTestCase

@end

@implementation ae_range_selector_tests

- (void)test_region {
    auto const player = std::make_shared<test_utils::player_stub>();
    auto const deselector = std::make_shared<test_utils::deselector_stub>();

    std::vector<std::optional<ui::region>> received;

    range_selector selector{player.get(), deselector.get()};

    auto canceller =
        selector.observe_region([&received](std::optional<ui::region> const &region) { received.emplace_back(region); })
            .sync();

    XCTAssertFalse(selector.region().has_value());
    XCTAssertEqual(received.size(), 1);
    XCTAssertFalse(received.at(0).has_value());

    selector.begin({.x = 1.0f, .y = 2.0f});

    {
        ui::region const expected{.origin = {.x = 1.0f, .y = 2.0f}, .size = ui::size::zero()};
        XCTAssertTrue(selector.region().value() == expected);
        XCTAssertEqual(received.size(), 2);
        XCTAssertTrue(received.at(1).value() == expected);
    }

    selector.move({.x = 2.0f, .y = 4.0f});

    {
        ui::region const expected{.origin = {.x = 1.0f, .y = 2.0f}, .size = {.width = 1.0f, .height = 2.0f}};
        XCTAssertTrue(selector.region().value() == expected);
        XCTAssertEqual(received.size(), 3);
        XCTAssertTrue(received.at(2).value() == expected);
    }

    selector.move({.x = 0.0f, .y = 0.0f});

    {
        ui::region const expected{.origin = {.x = 0.0f, .y = 0.0f}, .size = {.width = 1.0f, .height = 2.0f}};
        XCTAssertTrue(selector.region().value() == expected);
        XCTAssertEqual(received.size(), 4);
        XCTAssertTrue(received.at(3).value() == expected);
    }

    selector.move({.x = -1.0f, .y = -2.0f});

    {
        ui::region const expected{.origin = {.x = -1.0f, .y = -2.0f}, .size = {.width = 2.0f, .height = 4.0f}};
        XCTAssertTrue(selector.region().value() == expected);
        XCTAssertEqual(received.size(), 5);
        XCTAssertTrue(received.at(4).value() == expected);
    }

    selector.end();

    XCTAssertFalse(selector.region().has_value());
    XCTAssertEqual(received.size(), 6);
    XCTAssertFalse(received.at(5).has_value());

    canceller->cancel();
}

- (void)test_cannot_move_if_not_began {
    auto const player = std::make_shared<test_utils::player_stub>();
    auto const deselector = std::make_shared<test_utils::deselector_stub>();

    range_selector selector{player.get(), deselector.get()};

    selector.move({.x = 2.0f, .y = 4.0f});

    XCTAssertFalse(selector.region().has_value());
}

- (void)test_cannot_begin_if_playing {
    auto const player = std::make_shared<test_utils::player_stub>();
    auto const deselector = std::make_shared<test_utils::deselector_stub>();

    player->set_is_playing(true);

    range_selector selector{player.get(), deselector.get()};

    XCTAssertFalse(selector.region().has_value());

    selector.begin({.x = 1.0f, .y = 2.0f});

    XCTAssertFalse(selector.region().has_value());
}

- (void)test_end_when_played {
    auto const player = std::make_shared<test_utils::player_stub>();
    auto const deselector = std::make_shared<test_utils::deselector_stub>();

    range_selector selector{player.get(), deselector.get()};

    selector.begin({.x = 1.0f, .y = 2.0f});

    XCTAssertTrue(selector.region().has_value());

    player->set_is_playing(true);

    XCTAssertFalse(selector.region().has_value());
}

@end

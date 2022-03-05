//
//  ae_responder_stack_tests.mm
//

#import <XCTest/XCTest.h>
#include <audio_editor_core/ae_responder_stack.h>
#include <vector>

using namespace yas;
using namespace yas::ae;

namespace yas::ae::test {
struct responder_stub : ae::responder {
    std::function<void(ae::action const &)> handle_action_handler = [](auto const &) {};
    std::function<responding(ae::action const &)> responding_to_action_handler = [](auto const &) {
        return responding::fallthrough;
    };

    identifier responder_id() override {
        return this->_raw_responder_id;
    }

    void handle_action(ae::action const &action) override {
        this->handle_action_handler(action);
    }

    responding responding_to_action(ae::action const &action) override {
        return this->responding_to_action_handler(action);
    }

   private:
    identifier const _raw_responder_id;
};
}

@interface ae_responder_stack_tests : XCTestCase

@end

@implementation ae_responder_stack_tests {
    std::shared_ptr<test::responder_stub> responder_1;
    std::shared_ptr<test::responder_stub> responder_2;
    std::shared_ptr<responder_stack> stack;

    std::vector<ae::action> responding_called_1;
    std::vector<ae::action> handle_called_1;
    responding responding_result_1;
    std::vector<ae::action> responding_called_2;
    std::vector<ae::action> handle_called_2;
    responding responding_result_2;
}

- (void)setUp {
    self->responder_1 = std::make_shared<test::responder_stub>();
    self->responder_2 = std::make_shared<test::responder_stub>();
    self->stack = responder_stack::make_shared();

    self->responding_result_1 = responding::fallthrough;
    self->responding_result_2 = responding::fallthrough;

    auto &responding_called_1 = self->responding_called_1;
    auto &responding_result_1 = self->responding_result_1;
    auto &handle_called_1 = self->handle_called_1;
    auto &responding_called_2 = self->responding_called_2;
    auto &responding_result_2 = self->responding_result_2;
    auto &handle_called_2 = self->handle_called_2;

    self->responder_1->responding_to_action_handler = [&responding_called_1, &responding_result_1](auto const &action) {
        responding_called_1.push_back(action);
        return responding_result_1;
    };

    self->responder_1->handle_action_handler = [&handle_called_1](auto const &action) {
        handle_called_1.push_back(action);
    };

    self->responder_2->responding_to_action_handler = [&responding_called_2, &responding_result_2](auto const &action) {
        responding_called_2.push_back(action);
        return responding_result_2;
    };

    self->responder_2->handle_action_handler = [&handle_called_2](auto const &action) {
        handle_called_2.push_back(action);
    };
}

- (void)tearDown {
    self->responder_1 = nullptr;
    self->responder_2 = nullptr;
    self->stack = nullptr;

    [self reset_called];
}

- (void)reset_called {
    self->responding_called_1.clear();
    self->handle_called_1.clear();
    self->responding_called_2.clear();
    self->handle_called_2.clear();
}

- (void)test_push_and_pop {
    auto const &responder_1 = self->responder_1;
    auto const &responder_2 = self->responder_2;
    auto const &stack = self->stack;

    XCTAssertNoThrow(stack->push_responder(responder_1));
    XCTAssertThrows(stack->push_responder(responder_1));

    XCTAssertNoThrow(stack->push_responder(responder_2));

    XCTAssertThrows(stack->pop_responder(responder_1->responder_id()));

    XCTAssertNoThrow(stack->pop_responder(responder_2->responder_id()));
    XCTAssertNoThrow(stack->pop_responder(responder_1->responder_id()));
}

- (void)test_responding_to_action {
    auto const &responder_1 = self->responder_1;
    auto const &responder_2 = self->responder_2;
    auto const &stack = self->stack;

    auto &responding_called_1 = self->responding_called_1;
    auto &responding_result_1 = self->responding_result_1;
    auto &responding_called_2 = self->responding_called_2;
    auto &responding_result_2 = self->responding_result_2;

    {
        // 何もpushしていない

        responding_result_1 = responding::accepting;
        responding_result_2 = responding::accepting;

        XCTAssertEqual(stack->responding_to_action((ae::action{ae::action_kind::begin_time_editing})),
                       responding::fallthrough);

        XCTAssertEqual(responding_called_1.size(), 0);
        XCTAssertEqual(responding_called_2.size(), 0);
    }

    [self reset_called];

    {
        // 1つだけpushされていてfallthrough

        stack->push_responder(responder_1);

        responding_result_1 = responding::fallthrough;
        responding_result_2 = responding::fallthrough;

        XCTAssertEqual(stack->responding_to_action((ae::action{ae::action_kind::finish_time_editing})),
                       responding::fallthrough);

        XCTAssertEqual(responding_called_1.size(), 1);
        XCTAssertEqual(responding_called_1.at(0), (ae::action{ae::action_kind::finish_time_editing}));
        XCTAssertEqual(responding_called_2.size(), 0);
    }

    [self reset_called];

    {
        // 2つpushされていて、両方fallthrough

        stack->push_responder(responder_2);

        XCTAssertEqual(stack->responding_to_action((ae::action{ae::action_kind::cancel_time_editing})),
                       responding::fallthrough);

        XCTAssertEqual(responding_called_1.size(), 1);
        XCTAssertEqual(responding_called_1.at(0), (ae::action{ae::action_kind::cancel_time_editing}));
        XCTAssertEqual(responding_called_2.size(), 1);
        XCTAssertEqual(responding_called_2.at(0), (ae::action{ae::action_kind::cancel_time_editing}));
    }

    [self reset_called];

    {
        // 2つpushされていて、2つ目がblocking

        responding_result_1 = responding::fallthrough;
        responding_result_2 = responding::blocking;

        XCTAssertEqual(stack->responding_to_action((ae::action{ae::action_kind::move_to_next_time_unit})),
                       responding::blocking);

        // 2つ目で防がれるので1つ目は呼び出されない
        XCTAssertEqual(responding_called_1.size(), 0);
        XCTAssertEqual(responding_called_2.size(), 1);
        XCTAssertEqual(responding_called_2.at(0), (ae::action{ae::action_kind::move_to_next_time_unit}));
    }

    [self reset_called];

    {
        // 2つpushされていて、2つ目がaccepting

        responding_result_1 = responding::fallthrough;
        responding_result_2 = responding::accepting;

        XCTAssertEqual(stack->responding_to_action((ae::action{ae::action_kind::move_to_previous_time_unit})),
                       responding::accepting);

        // 2つ目が応答するので1つ目は呼び出されない
        XCTAssertEqual(responding_called_1.size(), 0);
        XCTAssertEqual(responding_called_2.size(), 1);
        XCTAssertEqual(responding_called_2.at(0), (ae::action{ae::action_kind::move_to_previous_time_unit}));
    }

    [self reset_called];

    {
        // 2つpushされていて、1つ目がblocking

        responding_result_1 = responding::blocking;
        responding_result_2 = responding::fallthrough;

        XCTAssertEqual(stack->responding_to_action((ae::action{ae::action_kind::input_time})), responding::blocking);

        // 2つ目はスルーされ、1つ目が呼び出される
        XCTAssertEqual(responding_called_1.size(), 1);
        XCTAssertEqual(responding_called_1.at(0), (ae::action{ae::action_kind::input_time}));
        XCTAssertEqual(responding_called_2.size(), 1);
        XCTAssertEqual(responding_called_2.at(0), (ae::action{ae::action_kind::input_time}));
    }

    [self reset_called];
}

- (void)test_handle_action {
    auto const &responder_1 = self->responder_1;
    auto const &responder_2 = self->responder_2;
    auto const &stack = self->stack;

    auto &responding_called_1 = self->responding_called_1;
    auto &responding_result_1 = self->responding_result_1;
    auto &handle_called_1 = self->handle_called_1;
    auto &responding_called_2 = self->responding_called_2;
    auto &responding_result_2 = self->responding_result_2;
    auto &handle_called_2 = self->handle_called_2;

    stack->push_responder(responder_1);
    stack->push_responder(responder_2);

    {
        // 両方fallthrough

        responding_result_1 = responding::fallthrough;
        responding_result_2 = responding::fallthrough;

        XCTAssertNoThrow(stack->handle_action({ae::action_kind::begin_time_editing}));

        XCTAssertEqual(responding_called_1.size(), 1);
        XCTAssertEqual(responding_called_1.at(0), (ae::action{ae::action_kind::begin_time_editing}));
        XCTAssertEqual(handle_called_1.size(), 0);
        XCTAssertEqual(responding_called_2.size(), 1);
        XCTAssertEqual(responding_called_2.at(0), (ae::action{ae::action_kind::begin_time_editing}));
        XCTAssertEqual(handle_called_2.size(), 0);
    }

    [self reset_called];

    {
        // 2つ目がblocking

        responding_result_1 = responding::fallthrough;
        responding_result_2 = responding::blocking;

        XCTAssertNoThrow(stack->handle_action({ae::action_kind::finish_time_editing}));

        XCTAssertEqual(responding_called_1.size(), 0);
        XCTAssertEqual(handle_called_1.size(), 0);
        XCTAssertEqual(responding_called_2.size(), 1);
        XCTAssertEqual(responding_called_2.at(0), (ae::action{ae::action_kind::finish_time_editing}));
        XCTAssertEqual(handle_called_2.size(), 0);
    }

    [self reset_called];

    {
        // 2つ目がaccepting

        responding_result_1 = responding::fallthrough;
        responding_result_2 = responding::accepting;

        XCTAssertNoThrow(stack->handle_action({ae::action_kind::cancel_time_editing}));

        XCTAssertEqual(responding_called_1.size(), 0);
        XCTAssertEqual(handle_called_1.size(), 0);
        XCTAssertEqual(responding_called_2.size(), 1);
        XCTAssertEqual(responding_called_2.at(0), (ae::action{ae::action_kind::cancel_time_editing}));
        XCTAssertEqual(handle_called_2.size(), 1);
        XCTAssertEqual(handle_called_2.at(0), (ae::action{ae::action_kind::cancel_time_editing}));
    }

    [self reset_called];

    {
        // 1つ目がaccepting

        responding_result_1 = responding::accepting;
        responding_result_2 = responding::fallthrough;

        XCTAssertNoThrow(stack->handle_action({ae::action_kind::move_to_next_time_unit}));

        XCTAssertEqual(responding_called_1.size(), 1);
        XCTAssertEqual(responding_called_1.at(0), (ae::action{ae::action_kind::move_to_next_time_unit}));
        XCTAssertEqual(handle_called_1.size(), 1);
        XCTAssertEqual(handle_called_1.at(0), (ae::action{ae::action_kind::move_to_next_time_unit}));
        XCTAssertEqual(responding_called_2.size(), 1);
        XCTAssertEqual(responding_called_1.at(0), (ae::action{ae::action_kind::move_to_next_time_unit}));
        XCTAssertEqual(handle_called_2.size(), 0);
    }

    [self reset_called];
}

@end

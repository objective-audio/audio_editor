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
    std::function<bool(ae::action const &)> responds_to_action_handler = [](auto const &) { return false; };

    identifier responder_id() override {
        return this->_raw_responder_id;
    }

    void handle_action(ae::action const &action) override {
        this->handle_action_handler(action);
    }

    bool responds_to_action(ae::action const &action) override {
        return this->responds_to_action_handler(action);
    }

   private:
    identifier const _raw_responder_id;
};
}

@interface ae_responder_stack_tests : XCTestCase

@end

@implementation ae_responder_stack_tests

- (void)test {
    auto const responder_1 = std::make_shared<test::responder_stub>();
    auto const responder_2 = std::make_shared<test::responder_stub>();
    auto const stack = responder_stack::make_shared();

    std::vector<ae::action> responds_to_action_called_1;
    std::vector<ae::action> handle_action_called_1;
    bool responds_to_action_result_1 = false;
    std::vector<ae::action> responds_to_action_called_2;
    std::vector<ae::action> handle_action_called_2;
    bool responds_to_action_result_2 = false;

    responder_1->responds_to_action_handler = [&responds_to_action_called_1,
                                               &responds_to_action_result_1](auto const &action) {
        responds_to_action_called_1.push_back(action);
        return responds_to_action_result_1;
    };

    responder_1->handle_action_handler = [&handle_action_called_1](auto const &action) {
        handle_action_called_1.push_back(action);
    };

    responder_2->responds_to_action_handler = [&responds_to_action_called_2,
                                               &responds_to_action_result_2](auto const &action) {
        responds_to_action_called_2.push_back(action);
        return responds_to_action_result_2;
    };

    responder_2->handle_action_handler = [&handle_action_called_2](auto const &action) {
        handle_action_called_2.push_back(action);
    };

    stack->push_responder(responder_1);

    responds_to_action_result_1 = false;
    responds_to_action_result_2 = false;

    XCTAssertFalse(stack->responds_to_action(ae::action{ae::action_kind::begin_time_editing}));

    XCTAssertEqual(handle_action_called_1.size(), 0);
    XCTAssertEqual(responds_to_action_called_1.size(), 1);
    XCTAssertEqual(responds_to_action_called_1.at(0), (ae::action{ae::action_kind::begin_time_editing}));
    XCTAssertEqual(handle_action_called_2.size(), 0);
    XCTAssertEqual(responds_to_action_called_2.size(), 0);

    responds_to_action_result_1 = true;
    responds_to_action_result_2 = false;

    XCTAssertTrue(stack->responds_to_action(ae::action{ae::action_kind::cancel_time_editing}));

    XCTAssertEqual(handle_action_called_1.size(), 0);
    XCTAssertEqual(responds_to_action_called_1.size(), 2);
    XCTAssertEqual(responds_to_action_called_1.at(1), (ae::action{ae::action_kind::cancel_time_editing}));
    XCTAssertEqual(handle_action_called_2.size(), 0);
    XCTAssertEqual(responds_to_action_called_2.size(), 0);

    stack->handle_action(ae::action{ae::action_kind::change_time_sign_to_minus});

    XCTAssertEqual(handle_action_called_1.size(), 1);
    XCTAssertEqual(handle_action_called_1.at(0), (ae::action{ae::action_kind::change_time_sign_to_minus}));
    XCTAssertEqual(responds_to_action_called_1.size(), 3);
    XCTAssertEqual(responds_to_action_called_1.at(2), (ae::action{ae::action_kind::change_time_sign_to_minus}));
    XCTAssertEqual(handle_action_called_2.size(), 0);
    XCTAssertEqual(responds_to_action_called_2.size(), 0);

    stack->push_responder(responder_2);

    responds_to_action_result_1 = true;
    responds_to_action_result_2 = true;

    XCTAssertTrue(stack->responds_to_action(ae::action{ae::action_kind::change_time_sign_to_plus}));

    XCTAssertEqual(handle_action_called_1.size(), 1);
    XCTAssertEqual(responds_to_action_called_1.size(), 3);
    XCTAssertEqual(handle_action_called_2.size(), 0);
    XCTAssertEqual(responds_to_action_called_2.size(), 1);
    XCTAssertEqual(responds_to_action_called_2.at(0), (ae::action{ae::action_kind::change_time_sign_to_plus}));

    stack->handle_action(ae::action{ae::action_kind::copy});

    XCTAssertEqual(handle_action_called_1.size(), 1);
    XCTAssertEqual(responds_to_action_called_1.size(), 3);
    XCTAssertEqual(handle_action_called_2.size(), 1);
    XCTAssertEqual(handle_action_called_2.at(0), (ae::action{ae::action_kind::copy}));
    XCTAssertEqual(responds_to_action_called_2.size(), 2);
    XCTAssertEqual(responds_to_action_called_2.at(1), (ae::action{ae::action_kind::copy}));

    responds_to_action_result_1 = true;
    responds_to_action_result_2 = false;

    XCTAssertTrue(stack->responds_to_action(ae::action{ae::action_kind::cut}));

    XCTAssertEqual(responds_to_action_called_1.size(), 4);
    XCTAssertEqual(responds_to_action_called_1.at(3), (ae::action{ae::action_kind::cut}));
    XCTAssertEqual(responds_to_action_called_2.size(), 3);
    XCTAssertEqual(responds_to_action_called_2.at(2), (ae::action{ae::action_kind::cut}));

    responds_to_action_result_1 = false;
    responds_to_action_result_2 = false;

    XCTAssertFalse(stack->responds_to_action(ae::action{ae::action_kind::decrement_time}));

    XCTAssertEqual(responds_to_action_called_1.size(), 5);
    XCTAssertEqual(responds_to_action_called_1.at(4), (ae::action{ae::action_kind::decrement_time}));
    XCTAssertEqual(responds_to_action_called_2.size(), 4);
    XCTAssertEqual(responds_to_action_called_2.at(3), (ae::action{ae::action_kind::decrement_time}));

    XCTAssertThrows(stack->pop_responder(responder_1->responder_id()));

    XCTAssertNoThrow(stack->pop_responder(responder_2->responder_id()));

    responds_to_action_result_1 = true;
    responds_to_action_result_2 = true;

    XCTAssertTrue(stack->responds_to_action(ae::action{ae::action_kind::delete_time}));

    XCTAssertEqual(responds_to_action_called_1.size(), 6);
    XCTAssertEqual(responds_to_action_called_1.at(5), (ae::action{ae::action_kind::delete_time}));
    XCTAssertEqual(responds_to_action_called_2.size(), 4);

    XCTAssertNoThrow(stack->pop_responder(responder_1->responder_id()));

    XCTAssertFalse(stack->responds_to_action(ae::action{ae::action_kind::drop_head}));

    XCTAssertEqual(responds_to_action_called_1.size(), 6);
    XCTAssertEqual(responds_to_action_called_2.size(), 4);
}

@end

//
//  ae_responder_stack.cpp
//

#include "ae_responder_stack.h"

using namespace yas;
using namespace yas::ae;

std::shared_ptr<responder_stack> responder_stack::make_shared() {
    return std::shared_ptr<responder_stack>(new responder_stack{});
}

responder_stack::responder_stack() {
}

void responder_stack::handle_action(ae::action const &action) {
    for (auto const &pair : this->_responders) {
        if (auto const &responder = pair.second.lock()) {
            auto const responding = responder->responding_to_action(action);
            switch (responding) {
                case responding::accepting:
                    responder->handle_action(action);
                    return;
                case responding::blocking:
                    return;
                case responding::fallthrough:
                    break;
            }
        }
    }
}

responding responder_stack::responding_to_action(ae::action const &action) {
    for (auto const &pair : this->_responders) {
        if (auto const &responder = pair.second.lock()) {
            auto const responding = responder->responding_to_action(action);
            switch (responding) {
                case responding::accepting:
                    return responding::accepting;
                case responding::blocking:
                    return responding::blocking;
                case responding::fallthrough:
                    break;
            }
        }
    }
    return responding::fallthrough;
}

void responder_stack::push_responder(std::shared_ptr<ae::responder> const &responder) {
    for (auto const &pair : this->_responders) {
        if (pair.first == responder->responder_id()) {
            throw std::runtime_error("responder exists.");
        }
    }
    this->_responders.emplace_front(responder->responder_id(), responder);
}

void responder_stack::pop_responder(identifier const &responder_id) {
    if (this->_responders.size() == 0) {
        throw std::runtime_error("responders is empry.");
    }

    auto const &front = this->_responders.front();

    if (front.first == responder_id) {
        this->_responders.pop_front();
    } else {
        throw std::runtime_error("responder_id is not equal to last.");
    }
}

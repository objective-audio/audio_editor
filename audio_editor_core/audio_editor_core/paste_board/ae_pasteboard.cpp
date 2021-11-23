//
//  ae_pasteboard.cpp
//

#include "ae_pasteboard.h"

using namespace yas;
using namespace yas::ae;

std::shared_ptr<pasteboard> pasteboard::make_shared() {
    return std::shared_ptr<pasteboard>(new pasteboard{});
}

pasteboard::pasteboard() {
}

std::optional<pasting_file_module> pasteboard::file_module() const {
    return pasting_file_module::make(this->_data);
}

void pasteboard::set_file_module(pasting_file_module const &module) {
    this->_data = module.data();
}

//
//  ae_project.cpp
//

#include "ae_project.h"

using namespace yas;
using namespace yas::ae;

project::project(url const &file_url) : _file_url(file_url) {
}

project_ptr project::make_shared(url const &file_url) {
    return std::shared_ptr<project>(new project{file_url});
}

uintptr_t project::identifier() const {
    return reinterpret_cast<uintptr_t>(this);
}

url const &project::file_url() const {
    return this->_file_url;
}

bool project::can_close() const {
    return true;
}

void project::request_close() {
    this->_notifier->notify(notification::shouldClose);
}

observing::endable project::observe_notify(std::function<void(notification const &)> &&handler) {
    return this->_notifier->observe(std::move(handler));
}

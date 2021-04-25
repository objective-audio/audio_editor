//
//  ae_project.cpp
//

#include "ae_project.h"

using namespace yas;
using namespace yas::ae;

project::project(std::string const &identifier, url const &file_url) : _identifier(identifier), _file_url(file_url) {
}

std::shared_ptr<project> project::make_shared(std::string const &identifier, url const &file_url) {
    return std::shared_ptr<project>(new project{identifier, file_url});
}

std::string const &project::identifier() const {
    return this->_identifier;
}

url const &project::file_url() const {
    return this->_file_url;
}

project_state const &project::state() const {
    return this->_state->value();
}

bool project::can_close() const {
    return true;
}

void project::request_close() {
    this->_notifier->notify(project_event::should_close);
}

observing::syncable project::observe_state(std::function<void(project_state const &)> &&handler) {
    return this->_state->observe(std::move(handler));
}

observing::endable project::observe_event(std::function<void(project_event const &)> &&handler) {
    return this->_notifier->observe(std::move(handler));
}

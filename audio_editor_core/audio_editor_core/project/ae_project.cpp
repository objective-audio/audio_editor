//
//  ae_project.cpp
//

#include "ae_project.h"

#include <audio_editor_core/ae_app.h>
#include <audio_editor_core/ae_file_importer.h>
#include <audio_editor_core/ae_system_url.h>
#include <cpp_utils/yas_file_manager.h>

using namespace yas;
using namespace yas::ae;

project::project(std::string const &identifier, url const &file_url,
                 std::shared_ptr<project_url_interface> const &project_url,
                 std::shared_ptr<project_file_importer_interface> const &file_importer)
    : _identifier(identifier),
      _file_url(file_url),
      _project_url(project_url),
      _file_importer(file_importer),
      _state(observing::value::holder<project_state>::make_shared(project_state::launching)),
      _notifier(observing::notifier<project_event>::make_shared()) {
    this->_setup();
}

std::shared_ptr<project> project::make_shared(std::string const &identifier, url const &file_url) {
    auto const project_url = project_url::make_shared(app::global()->system_url()->project_directory(identifier));
    auto const file_importer = app::global()->file_importer();
    return make_shared(identifier, file_url, project_url, file_importer);
}

std::shared_ptr<project> project::make_shared(std::string const &identifier, url const &file_url,
                                              std::shared_ptr<project_url_interface> const &project_url,
                                              std::shared_ptr<project_file_importer_interface> const &file_importer) {
    return std::shared_ptr<project>(new project{identifier, file_url, project_url, file_importer});
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
    switch (this->_state->value()) {
        case project_state::closing:
            return;
        case project_state::loading:
            this->_file_importer->cancel(this->_identifier);
            [[fallthrough]];
        case project_state::launching:
        case project_state::editing:
        case project_state::failure:
            this->_state->set_value(project_state::closing);
            break;
    }

    this->_notifier->notify(project_event::should_close);
}

observing::syncable project::observe_state(std::function<void(project_state const &)> &&handler) {
    return this->_state->observe(std::move(handler));
}

observing::endable project::observe_event(std::function<void(project_event const &)> &&handler) {
    return this->_notifier->observe(std::move(handler));
}

void project::_setup() {
    this->_state->set_value(project_state::loading);

    this->_file_importer->import({.identifier = this->_identifier,
                                  .src_url = this->_file_url,
                                  .dst_url = this->_project_url->editing_file(),
                                  .completion = [weak_state = to_weak(this->_state)](bool const result) {
                                      if (auto const state = weak_state.lock()) {
                                          switch (state->value()) {
                                              case project_state::loading: {
                                                  if (result) {
                                                      state->set_value(project_state::editing);
                                                  } else {
                                                      state->set_value(project_state::failure);
                                                  }
                                              } break;

                                              case project_state::launching:
                                              case project_state::editing:
                                              case project_state::failure:
                                              case project_state::closing:
                                                  break;
                                          }
                                      }
                                  }});
}

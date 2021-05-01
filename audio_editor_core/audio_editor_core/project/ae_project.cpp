//
//  ae_project.cpp
//

#include "ae_project.h"

#include <audio_editor_core/ae_app.h>
#include <audio_editor_core/ae_file_importer.h>
#include <audio_editor_core/ae_file_loader.h>
#include <audio_editor_core/ae_system_url.h>
#include <cpp_utils/yas_file_manager.h>

using namespace yas;
using namespace yas::ae;

project::project(std::string const &identifier, url const &file_url,
                 std::shared_ptr<project_url_interface> const &project_url,
                 std::shared_ptr<project_file_importer_interface> const &file_importer,
                 std::shared_ptr<project_file_loader_interface> const &file_loader)
    : _identifier(identifier),
      _file_url(file_url),
      _project_url(project_url),
      _file_importer(file_importer),
      _file_loader(file_loader),
      _state(observing::value::holder<project_state>::make_shared(project_state::launching)),
      _file_info(observing::value::holder<std::optional<ae::file_info>>::make_shared(std::nullopt)),
      _event_notifier(observing::notifier<project_event>::make_shared()) {
}

std::shared_ptr<project> project::make_shared(std::string const &identifier, url const &file_url) {
    auto const project_url = project_url::make_shared(app::global()->system_url()->project_directory(identifier));
    auto const file_importer = app::global()->file_importer();
    auto const file_loader = file_loader::make_shared();
    return make_shared(identifier, file_url, project_url, file_importer, file_loader);
}

std::shared_ptr<project> project::make_shared(std::string const &identifier, url const &file_url,
                                              std::shared_ptr<project_url_interface> const &project_url,
                                              std::shared_ptr<project_file_importer_interface> const &file_importer,
                                              std::shared_ptr<project_file_loader_interface> const &file_loader) {
    auto shared = std::shared_ptr<project>(new project{identifier, file_url, project_url, file_importer, file_loader});
    shared->_setup(shared);
    return shared;
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

std::optional<file_info> project::file_info() const {
    return this->_file_info->value();
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

    this->_event_notifier->notify(project_event::should_close);
}

observing::syncable project::observe_state(std::function<void(project_state const &)> &&handler) {
    return this->_state->observe(std::move(handler));
}

observing::syncable project::observe_file_info(std::function<void(std::optional<ae::file_info>)> &&handler) {
    return this->_file_info->observe(std::move(handler));
}

observing::endable project::observe_event(std::function<void(project_event const &)> &&handler) {
    return this->_event_notifier->observe(std::move(handler));
}

void project::_setup(std::weak_ptr<project> weak) {
    this->_state->set_value(project_state::loading);

    this->_file_importer->import({.identifier = this->_identifier,
                                  .src_url = this->_file_url,
                                  .dst_url = this->_project_url->editing_file(),
                                  .completion = [weak](bool const result) {
                                      if (auto const project = weak.lock()) {
                                          auto const &state = project->_state->value();
                                          switch (state) {
                                              case project_state::loading: {
                                                  if (result) {
                                                      auto const dst_url = project->_project_url->editing_file();
                                                      project->_file_info->set_value(
                                                          project->_file_loader->load_file_info(dst_url));
                                                      if (project->_file_info->value().has_value()) {
                                                          project->_state->set_value(project_state::editing);
                                                      } else {
                                                          project->_state->set_value(project_state::failure);
                                                      }
                                                  } else {
                                                      project->_state->set_value(project_state::failure);
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

//
//  ae_project.cpp
//

#include "ae_project.h"

#include <audio_editor_core/ae_hierarchy.h>
#include <audio_editor_core/ae_project_editor.h>
#include <audio_editor_core/ae_responder_stack.h>
#include <audio_editor_core/ae_zooming.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<project> project::make_shared(
    std::string const &identifier, url const &file_url, std::shared_ptr<project_url_for_project> const &project_url,
    std::shared_ptr<file_importer_for_project> const &file_importer,
    std::shared_ptr<file_loader_for_project> const &file_loader,
    std::shared_ptr<project_editor_level_pool_for_project> const &editor_level_pool) {
    auto const &app_level = hierarchy::app_level();
    auto project = std::shared_ptr<ae::project>(new ae::project{
        identifier, file_url, project_url, file_importer, file_loader, app_level->responder_stack, editor_level_pool});
    project->_weak_project = project;
    return project;
}

project::project(std::string const &identifier, url const &file_url,
                 std::shared_ptr<project_url_for_project> const &project_url,
                 std::shared_ptr<file_importer_for_project> const &file_importer,
                 std::shared_ptr<file_loader_for_project> const &file_loader,
                 std::shared_ptr<responder_stack> const &responder_stack,
                 std::shared_ptr<project_editor_level_pool_for_project> const &editor_level_pool)
    : _identifier(identifier),
      _file_url(file_url),
      _project_url(project_url),
      _file_importer(file_importer),
      _file_loader(file_loader),
      _responder_stack(responder_stack),
      _editor_level_pool(editor_level_pool),
      _state(observing::value::holder<project_state>::make_shared(project_state::launching)),
      _event_notifier(observing::notifier<project_event>::make_shared()) {
}

void project::setup() {
    this->_state->set_value(project_state::loading);

    this->_file_importer->import(
        {.identifier = this->_identifier,
         .src_url = this->_file_url,
         .dst_url = this->_project_url->editing_file(),
         .completion = [weak = this->_weak_project](bool const result) {
             if (auto const project = weak.lock()) {
                 auto const &state = project->_state->value();
                 switch (state) {
                     case project_state::loading: {
                         if (result) {
                             auto const editing_file_url = project->_project_url->editing_file();
                             if (auto const file_info = project->_file_loader->load_file_info(editing_file_url)) {
                                 project->_editor_level_pool->add_level(file_info.value());

                                 auto const responder_stack = project->_responder_stack.lock();
                                 auto const editor_level = project->_editor_level_pool->level();
                                 if (responder_stack && editor_level) {
                                     responder_stack->push_responder(editor_level->editor);
                                 }

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
            this->_state->set_value(project_state::closing);
            break;
        case project_state::editing:
            this->_editor_level_pool->remove_level();
            this->_state->set_value(project_state::closing);
            break;
        case project_state::launching:
        case project_state::failure:
            this->_state->set_value(project_state::closing);
            break;
    }

    this->_event_notifier->notify(project_event::should_close);
}

observing::syncable project::observe_state(std::function<void(project_state const &)> &&handler) {
    return this->_state->observe(std::move(handler));
}

observing::endable project::observe_event(std::function<void(project_event const &)> &&handler) {
    return this->_event_notifier->observe(std::move(handler));
}

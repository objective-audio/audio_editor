//
//  ae_project.cpp
//

#include "ae_project.h"

using namespace yas;
using namespace yas::ae;

project::project(std::string const &identifier, url const &file_url,
                 std::shared_ptr<project_url_for_project> const &project_url,
                 std::shared_ptr<file_importer_for_project> const &file_importer,
                 std::shared_ptr<file_loader_for_project> const &file_loader,
                 std::shared_ptr<player_for_project> const &player,
                 std::shared_ptr<project_editor_maker_for_project> const &editor_maker,
                 std::shared_ptr<zooming_for_project> const &horizontal_zooming,
                 std::shared_ptr<scrolling_for_project> const &scrolling,
                 std::shared_ptr<ae::action_controller> const &action_controller,
                 std::shared_ptr<ae::dialog_presenter> const &dialog_presenter)
    : _identifier(identifier),
      _file_url(file_url),
      _project_url(project_url),
      _file_importer(file_importer),
      _file_loader(file_loader),
      _player(player),
      _editor_maker(editor_maker),
      _horizontal_zooming(horizontal_zooming),
      _scrolling(scrolling),
      _action_controller(action_controller),
      _dialog_presenter(dialog_presenter),
      _state(observing::value::holder<project_state>::make_shared(project_state::launching)),
      _event_notifier(observing::notifier<project_event>::make_shared()) {
}

std::shared_ptr<project> project::make_shared(std::string const &identifier, url const &file_url,
                                              std::shared_ptr<project_url_for_project> const &project_url,
                                              std::shared_ptr<file_importer_for_project> const &file_importer,
                                              std::shared_ptr<file_loader_for_project> const &file_loader,
                                              std::shared_ptr<player_for_project> const &player,
                                              std::shared_ptr<project_editor_maker_for_project> const &editor_maker,
                                              std::shared_ptr<zooming_for_project> const &horizontal_zooming,
                                              std::shared_ptr<scrolling_for_project> const &scrolling,
                                              std::shared_ptr<ae::action_controller> const &action_controller,
                                              std::shared_ptr<ae::dialog_presenter> const &dialog_presenter) {
    auto shared = std::shared_ptr<project>(new project{identifier, file_url, project_url, file_importer, file_loader,
                                                       player, editor_maker, horizontal_zooming, scrolling,
                                                       action_controller, dialog_presenter});
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

std::shared_ptr<project_url_for_project> const &project::project_url() const {
    return this->_project_url;
}

std::shared_ptr<player_for_project> const &project::player() const {
    return this->_player;
}

std::shared_ptr<project_editor_for_project> const &project::editor() const {
    return this->_editor;
}

std::shared_ptr<zooming_for_project> const &project::horizontal_zooming() const {
    return this->_horizontal_zooming;
}

std::shared_ptr<scrolling_for_project> const &project::scrolling() const {
    return this->_scrolling;
}

std::shared_ptr<action_controller> const &project::action_controller() const {
    return this->_action_controller;
}

std::shared_ptr<ae::dialog_presenter> const &project::dialog_presenter() const {
    return this->_dialog_presenter;
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

observing::endable project::observe_event(std::function<void(project_event const &)> &&handler) {
    return this->_event_notifier->observe(std::move(handler));
}

void project::_setup(std::weak_ptr<project> weak) {
    this->_state->set_value(project_state::loading);

    this->_file_importer->import(
        {.identifier = this->_identifier,
         .src_url = this->_file_url,
         .dst_url = this->_project_url->editing_file(),
         .completion = [weak](bool const result) {
             if (auto const project = weak.lock()) {
                 auto const &state = project->_state->value();
                 switch (state) {
                     case project_state::loading: {
                         if (result) {
                             auto const &project_url = project->_project_url;
                             auto const editing_file_url = project_url->editing_file();
                             if (auto const file_info = project->_file_loader->load_file_info(editing_file_url)) {
                                 project->_editor = project->_editor_maker->make(
                                     editing_file_url, project_url->db_file(), file_info.value());
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

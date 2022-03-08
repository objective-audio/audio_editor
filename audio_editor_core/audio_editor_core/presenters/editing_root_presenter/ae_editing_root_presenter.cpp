//
//  ae_editing_root_presenter.cpp
//

#include "ae_editing_root_presenter.h"

#include <audio_editor_core/ae_editing_root_presenter_utils.h>
#include <audio_editor_core/ae_file_track.h>
#include <audio_editor_core/ae_hierarchy.h>
#include <audio_editor_core/ae_marker_pool.h>
#include <audio_editor_core/ae_player.h>
#include <audio_editor_core/ae_project.h>
#include <audio_editor_core/ae_project_editor.h>
#include <audio_editor_core/ae_responder_stack.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<editing_root_presenter> editing_root_presenter::make_shared(std::string const &project_id) {
    auto const &app_level = hierarchy::app_level();
    auto const &project_level = hierarchy::project_level_for_id(project_id);
    auto const &editor_level = hierarchy::project_editor_level_for_id(project_id);
    return std::shared_ptr<editing_root_presenter>(new editing_root_presenter{
        editor_level->file_info, project_level->player, editor_level->file_track, editor_level->marker_pool,
        project_level->project, editor_level->editor, app_level->responder_stack});
}

editing_root_presenter::editing_root_presenter(file_info const &file_info, std::shared_ptr<player> const &player,
                                               std::shared_ptr<file_track> const &file_track,
                                               std::shared_ptr<marker_pool> const &marker_pool,
                                               std::shared_ptr<project> const &project,
                                               std::shared_ptr<project_editor> const &editor,
                                               std::shared_ptr<responder_stack> const &responder_stack)
    : _file_info(file_info),
      _project(project),
      _player(player),
      _file_track(file_track),
      _marker_pool(marker_pool),
      _project_editor(editor),
      _responder_stack(responder_stack),
      _file_track_event_fetcher(editing_root_presenter_utils::make_file_track_fetcher(file_track)),
      _marker_pool_event_fetcher(editing_root_presenter_utils::make_marker_pool_fetcher(marker_pool)) {
    file_track->observe_event([this](auto const &event) { this->_file_track_event_fetcher->push(event); })
        .sync()
        ->add_to(this->_pool);

    marker_pool->observe_event([this](auto const &event) { this->_marker_pool_event_fetcher->push(event); })
        .end()
        ->add_to(this->_pool);
}

std::string editing_root_presenter::state_text() const {
    if (auto const project = this->_project.lock()) {
        return editing_root_presenter_utils::label_text(project->state());
    } else {
        return editing_root_presenter_utils::empty_text();
    }
}

std::string editing_root_presenter::file_info_text() const {
    return editing_root_presenter_utils::label_text(this->_file_info);
}

std::string editing_root_presenter::file_track_text() const {
    if (auto const file_track = this->_file_track.lock()) {
        return editing_root_presenter_utils::file_track_text(file_track->modules());
    } else {
        return editing_root_presenter_utils::empty_text();
    }
}

std::string editing_root_presenter::marker_pool_text() const {
    if (auto const marker_pool = this->_marker_pool.lock()) {
        return editing_root_presenter_utils::marker_pool_text(marker_pool->markers());
    } else {
        return editing_root_presenter_utils::empty_text();
    }
}

playing_line_state_t editing_root_presenter::playing_line_state() const {
    if (auto const player = this->_player.lock()) {
        if (player->is_scrolling()) {
            return playing_line_state_t::scrolling;
        } else if (player->is_playing()) {
            return playing_line_state_t::playing;
        }
    }

    return playing_line_state_t::pausing;
}

observing::syncable editing_root_presenter::observe_state_text(std::function<void(std::string const &)> &&handler) {
    if (auto const project = this->_project.lock()) {
        return project->observe_state([handler = std::move(handler)](project_state const &state) {
            handler(editing_root_presenter_utils::label_text(state));
        });
    } else {
        return observing::syncable{};
    }
}

observing::syncable editing_root_presenter::observe_file_track_text(
    std::function<void(std::string const &)> &&handler) {
    return this->_file_track_event_fetcher->observe(
        [handler = std::move(handler), this](auto const &event) { handler(this->file_track_text()); });
}

observing::syncable editing_root_presenter::observe_marker_pool_text(
    std::function<void(std::string const &)> &&handler) {
    return this->_marker_pool_event_fetcher->observe(
        [handler = std::move(handler), this](auto const &event) { handler(this->marker_pool_text()); });
}

bool editing_root_presenter::responds_to_action(action const action) {
    if (auto const responder_stack = this->_responder_stack.lock()) {
        auto const responding = responder_stack->responding_to_action(action);
        switch (responding) {
            case responding::accepting:
            case responding::blocking:
                return true;

            case responding::fallthrough:
                return false;
        }
    } else {
        return false;
    }
}

//
//  ae_project.cpp
//

#include "ae_project.h"

#include <audio_editor_core/ae_app.h>
#include <audio_editor_core/ae_file_importer.h>
#include <audio_editor_core/ae_file_loader.h>
#include <audio_editor_core/ae_player.h>
#include <audio_editor_core/ae_project_editor_maker.h>
#include <audio_editor_core/ae_scrolling.h>
#include <audio_editor_core/ae_system_url.h>
#include <audio_editor_core/ae_zooming.h>
#include <cpp_utils/yas_file_manager.h>

using namespace yas;
using namespace yas::ae;

project::project(std::string const &identifier, url const &file_url,
                 std::shared_ptr<project_url_for_project> const &project_url,
                 std::shared_ptr<file_importer_for_project> const &file_importer,
                 std::shared_ptr<file_loader_for_project> const &file_loader,
                 std::shared_ptr<player_for_project> const &player,
                 std::shared_ptr<project_editor_maker_for_project> const &editor_maker,
                 std::shared_ptr<scrolling_for_project> const &scrolling)
    : _identifier(identifier),
      _file_url(file_url),
      _project_url(project_url),
      _file_importer(file_importer),
      _file_loader(file_loader),
      _player(player),
      _editor_maker(editor_maker),
      _zooming(zooming::make_shared()),
      _scrolling(scrolling),
      _state(observing::value::holder<project_state>::make_shared(project_state::launching)),
      _file_info(observing::value::holder<std::optional<ae::file_info>>::make_shared(std::nullopt)),
      _event_notifier(observing::notifier<project_event>::make_shared()) {
}

std::shared_ptr<project> project::make_shared(std::string const &identifier, url const &file_url) {
    auto const app = app::global();
    auto const project_url = project_url::make_shared(app->system_url()->project_directory(identifier));
    auto const file_importer = app->file_importer();
    auto const file_loader = app->file_loader();
    auto const scrolling = scrolling::make_shared();
    auto const player = player::make_shared(app->system_url()->playing_directory(), identifier, scrolling);
    auto const editor_maker = project_editor_maker::make_shared(player);
    return make_shared(identifier, file_url, project_url, file_importer, file_loader, player, editor_maker, scrolling);
}

std::shared_ptr<project> project::make_shared(std::string const &identifier, url const &file_url,
                                              std::shared_ptr<project_url_for_project> const &project_url,
                                              std::shared_ptr<file_importer_for_project> const &file_importer,
                                              std::shared_ptr<file_loader_for_project> const &file_loader,
                                              std::shared_ptr<player_for_project> const &player,
                                              std::shared_ptr<project_editor_maker_for_project> const &editor_maker,
                                              std::shared_ptr<scrolling_for_project> const &scrolling) {
    auto shared = std::shared_ptr<project>(
        new project{identifier, file_url, project_url, file_importer, file_loader, player, editor_maker, scrolling});
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

std::optional<file_info> const &project::file_info() const {
    return this->_file_info->value();
}

std::shared_ptr<player_for_project> const &project::player() const {
    return this->_player;
}

std::shared_ptr<project_editor_for_project> const &project::editor() const {
    return this->_editor;
}

std::shared_ptr<zooming> const &project::zooming() const {
    return this->_zooming;
}

std::shared_ptr<scrolling_for_project> const &project::scrolling() const {
    return this->_scrolling;
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

bool project::can_nudge() const {
    return !this->player()->is_playing();
}

void project::nudge_previous() {
    if (!this->can_nudge()) {
        return;
    }

    frame_index_t const current_frame = this->player()->current_frame();
    this->player()->seek(current_frame - 1);
}

void project::nudge_next() {
    if (!this->can_nudge()) {
        return;
    }

    frame_index_t const current_frame = this->player()->current_frame();
    this->player()->seek(current_frame + 1);
}

bool project::can_jump_to_previous_edge() const {
    return this->_previous_edge().has_value();
}

bool project::can_jump_to_next_edge() const {
    return this->_next_edge().has_value();
}

void project::jump_to_previous_edge() {
    if (auto const edge = this->_previous_edge()) {
        this->player()->seek(edge.value());
    }
}

void project::jump_to_next_edge() {
    if (auto const edge = this->_next_edge()) {
        this->player()->seek(edge.value());
    }
}

bool project::can_split() const {
    if (auto const &editor = this->editor()) {
        auto const &file_track = editor->file_track();
        auto const current_frame = this->player()->current_frame();
        return file_track->splittable_module_at(current_frame).has_value();
    } else {
        return false;
    }
}

void project::split() {
    auto const current_frame = this->player()->current_frame();
    this->editor()->file_track()->split_at(current_frame);
}

void project::drop_head_and_offset() {
    auto const current_frame = this->player()->current_frame();
    this->editor()->file_track()->drop_head_and_offset_at(current_frame);
}

void project::drop_tail_and_offset() {
    auto const current_frame = this->player()->current_frame();
    this->editor()->file_track()->drop_tail_and_offset_at(current_frame);
}

bool project::can_erase() const {
    auto const current_frame = this->player()->current_frame();
    return this->editor()->file_track()->module_at(current_frame).has_value();
}

void project::erase_and_offset() {
    auto const current_frame = this->player()->current_frame();
    this->editor()->file_track()->erase_and_offset_at(current_frame);
}

bool project::can_insert_marker() const {
    auto const current_frame = this->player()->current_frame();
    return this->editor()->marker_pool()->markers().count(current_frame) == 0;
}

void project::insert_marker() {
    auto const current_frame = this->player()->current_frame();
    this->editor()->marker_pool()->insert_marker(marker{.frame = current_frame});
}

bool project::can_return_to_zero() const {
    return this->player()->current_frame() != 0;
}

void project::return_to_zero() {
    this->player()->seek(0);
}

bool project::can_go_to_marker(std::size_t const marker_idx) const {
    auto const &marker_pool = this->editor()->marker_pool();
    if (auto const marker = marker_pool->marker_at(marker_idx)) {
        return this->player()->current_frame() != marker->frame;
    } else {
        return false;
    }
}

void project::go_to_marker(std::size_t const marker_idx) {
    auto const &marker_pool = this->editor()->marker_pool();
    if (auto const marker = marker_pool->marker_at(marker_idx)) {
        this->player()->seek(marker->frame);
    }
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
                             auto const dst_url = project->_project_url->editing_file();
                             project->_file_info->set_value(project->_file_loader->load_file_info(dst_url));
                             if (project->_file_info->value().has_value()) {
                                 project->_editor =
                                     project->_editor_maker->make(dst_url, project->_file_info->value().value());
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

std::optional<proc::frame_index_t> project::_previous_edge() const {
    frame_index_t const current_frame = this->player()->current_frame();
    auto const file_track_edge = this->editor()->file_track()->previous_edge(current_frame);
    auto const marker_pool_edge = this->editor()->marker_pool()->previous_edge(current_frame);

    if (file_track_edge.has_value() && marker_pool_edge.has_value()) {
        return std::max(file_track_edge.value(), marker_pool_edge.value());
    } else if (file_track_edge.has_value()) {
        return file_track_edge.value();
    } else if (marker_pool_edge.has_value()) {
        return marker_pool_edge.value();
    } else {
        return std::nullopt;
    }
}

std::optional<proc::frame_index_t> project::_next_edge() const {
    frame_index_t const current_frame = this->player()->current_frame();
    auto const file_track_edge = this->editor()->file_track()->next_edge(current_frame);
    auto const marker_pool_edge = this->editor()->marker_pool()->next_edge(current_frame);

    if (file_track_edge.has_value() && marker_pool_edge.has_value()) {
        return std::min(file_track_edge.value(), marker_pool_edge.value());
    } else if (file_track_edge.has_value()) {
        return file_track_edge.value();
    } else if (marker_pool_edge.has_value()) {
        return marker_pool_edge.value();
    } else {
        return std::nullopt;
    }
}

//
//  ae_project_editor.cpp
//

#include "ae_project_editor.h"

#include <audio_editor_core/ae_app.h>
#include <audio_editor_core/ae_database.h>
#include <audio_editor_core/ae_file_loader.h>
#include <audio_editor_core/ae_file_track.h>
#include <audio_editor_core/ae_marker_pool.h>
#include <audio_editor_core/ae_project_editor_utils.h>
#include <cpp_utils/yas_fast_each.h>
#include <processing/yas_processing_umbrella.h>

using namespace yas;
using namespace yas::ae;

project_editor::project_editor(url const &editing_file_url, ae::file_info const &file_info,
                               std::shared_ptr<player_for_project_editor> const &player,
                               std::shared_ptr<file_track_for_project_editor> const &file_track,
                               std::shared_ptr<marker_pool_for_project_editor> const &marker_pool,
                               std::shared_ptr<database_for_project_editor> const &database,
                               std::shared_ptr<action_controller> const &action_controller)
    : _editing_file_url(editing_file_url),
      _file_info(file_info),
      _player(player),
      _file_track(file_track),
      _timeline(proc::timeline::make_shared()),
      _track(proc::track::make_shared()),
      _marker_pool(marker_pool),
      _database(database),
      _action_controller(action_controller) {
    this->_timeline->insert_track(0, this->_track);
    this->_player->set_timeline(this->_timeline, file_info.sample_rate, audio::pcm_format::float32);

    this->_file_track
        ->observe_event([this](file_track_event const &event) {
            auto const &url = this->_editing_file_url;
            auto const &ch_count = this->_file_info.channel_count;

            switch (event.type) {
                case file_track_event_type::any:
                    this->_timeline->erase_track(0);
                    this->_track = proc::track::make_shared();

                    for (auto const &pair : event.modules) {
                        auto const &file_module = pair.second;
                        this->_track->push_back_module(project_editor_utils::make_module(file_module, url, ch_count),
                                                       file_module.range);
                        this->_database->add_module(file_module);
                    }

                    this->_timeline->insert_track(0, this->_track);
                    this->_database->save();
                    break;
                case file_track_event_type::reverted:
                    this->_timeline->erase_track(0);
                    this->_track = proc::track::make_shared();

                    for (auto const &pair : event.modules) {
                        auto const &file_module = pair.second;
                        this->_track->push_back_module(project_editor_utils::make_module(file_module, url, ch_count),
                                                       file_module.range);
                    }

                    this->_timeline->insert_track(0, this->_track);
                    break;
                case file_track_event_type::inserted:
                    if (auto const &track = this->_track) {
                        auto const &file_module = event.module.value();
                        track->push_back_module(project_editor_utils::make_module(file_module, url, ch_count),
                                                file_module.range);
                        this->_database->add_module(file_module);
                        this->_database->save();
                    }
                    break;
                case file_track_event_type::erased:
                    if (auto const &track = this->_track) {
                        auto const &file_module = event.module.value();
                        track->erase_modules_for_range(file_module.range);
                        this->_database->remove_module(file_module.range);
                        this->_database->save();
                    }
                    break;
            }
        })
        .sync()
        ->add_to(this->_pool);

    this->_marker_pool
        ->observe_event([this](marker_pool_event const &event) {
            switch (event.type) {
                case marker_pool_event_type::any:
                    if (event.markers.size() > 0) {
                        for (auto const &pair : event.markers) {
                            this->_database->add_marker(pair.second);
                        }
                        this->_database->save();
                    }
                    break;
                case marker_pool_event_type::reverted:
                    break;
                case marker_pool_event_type::inserted:
                    this->_database->add_marker(event.marker.value());
                    this->_database->save();
                    break;
                case marker_pool_event_type::erased:
                    this->_database->remove_marker(event.marker.value().frame);
                    this->_database->save();
                    break;
            }
        })
        .sync()
        ->add_to(this->_pool);

    this->_database
        ->observe_reverted([this] {
            std::vector<file_module> file_modules;

            for (auto const &pair : this->_database->modules()) {
                if (auto file_module = pair.second.file_module()) {
                    file_modules.emplace_back(std::move(file_module.value()));
                }
            }

            this->_file_track->revert_modules_and_notify(std::move(file_modules));

            std::vector<marker> markers;

            for (auto const &pair : this->_database->markers()) {
                if (auto const marker = pair.second.marker()) {
                    markers.emplace_back(std::move(marker.value()));
                }
            }

            this->_marker_pool->revert_markers(std::move(markers));
        })
        .end()
        ->add_to(this->_pool);

    this->_file_track->insert_module_and_notify(file_module{proc::time::range{0, file_info.length}, 0});

    action_controller
        ->observe_action([this](action const &action) {
            switch (action) {
                case action::toggle_play:
                    this->set_playing(!this->is_playing());
                    break;
                case action::nudge_previous:
                    this->nudge_previous();
                    break;
                case action::nudge_next:
                    this->nudge_next();
                    break;
                case action::jump_previous:
                    this->jump_to_previous_edge();
                    break;
                case action::jump_next:
                    this->jump_to_next_edge();
                    break;
                case action::drop_head_and_offset:
                    this->drop_head_and_offset();
                    break;
                case action::split:
                    this->split();
                    break;
                case action::drop_tail_and_offset:
                    this->drop_tail_and_offset();
                    break;
                case action::erase_and_offset:
                    this->erase_and_offset();
                    break;
                case action::insert_marker:
                    this->insert_marker();
                    break;
                case action::return_to_zero:
                    this->return_to_zero();
                    break;
                case action::go_to_marker_1:
                    this->go_to_marker(0);
                    break;
                case action::go_to_marker_2:
                    this->go_to_marker(1);
                    break;
                case action::go_to_marker_3:
                    this->go_to_marker(2);
                    break;
                case action::go_to_marker_4:
                    this->go_to_marker(3);
                    break;
                case action::go_to_marker_5:
                    this->go_to_marker(4);
                    break;
                case action::go_to_marker_6:
                    this->go_to_marker(5);
                    break;
                case action::go_to_marker_7:
                    this->go_to_marker(6);
                    break;
                case action::go_to_marker_8:
                    this->go_to_marker(7);
                    break;
                case action::go_to_marker_9:
                    this->go_to_marker(8);
                    break;
                case action::undo:
                    this->undo();
                    break;
                case action::redo:
                    this->redo();
                    break;
            }
        })
        .end()
        ->add_to(this->_pool);
}

ae::file_info const &project_editor::file_info() const {
    return this->_file_info;
}

std::shared_ptr<player_for_project_editor> const &project_editor::player() const {
    return this->_player;
}

std::shared_ptr<file_track_for_project_editor> const &project_editor::file_track() const {
    return this->_file_track;
}

std::shared_ptr<marker_pool_for_project_editor> const &project_editor::marker_pool() const {
    return this->_marker_pool;
}

std::shared_ptr<database_for_project_editor> const &project_editor::database() const {
    return this->_database;
}

proc::frame_index_t project_editor::current_frame() const {
    return this->_player->current_frame();
}

void project_editor::set_playing(bool const is_playing) {
    this->_player->set_playing(is_playing);
}

bool project_editor::is_playing() const {
    return this->_player->is_playing();
}

bool project_editor::is_scrolling() const {
    return this->_player->is_scrolling();
}

bool project_editor::can_nudge() const {
    return !this->_player->is_playing();
}

void project_editor::nudge_previous() {
    if (!this->can_nudge()) {
        return;
    }

    frame_index_t const current_frame = this->_player->current_frame();
    this->_player->seek(current_frame - 1);
}

void project_editor::nudge_next() {
    if (!this->can_nudge()) {
        return;
    }

    frame_index_t const current_frame = this->_player->current_frame();
    this->_player->seek(current_frame + 1);
}

bool project_editor::can_jump_to_previous_edge() const {
    return this->previous_edge().has_value();
}

bool project_editor::can_jump_to_next_edge() const {
    return this->next_edge().has_value();
}

void project_editor::jump_to_previous_edge() {
    if (auto const edge = this->previous_edge()) {
        this->_player->seek(edge.value());
    }
}

void project_editor::jump_to_next_edge() {
    if (auto const edge = this->next_edge()) {
        this->_player->seek(edge.value());
    }
}

bool project_editor::can_split() const {
    auto const &file_track = this->file_track();
    auto const current_frame = this->_player->current_frame();
    return file_track->splittable_module_at(current_frame).has_value();
}

void project_editor::split() {
    if (!this->can_split()) {
        return;
    }

    auto const current_frame = this->_player->current_frame();
    this->file_track()->split_at(current_frame);
}

void project_editor::drop_head_and_offset() {
    if (!this->can_split()) {
        return;
    }

    auto const current_frame = this->_player->current_frame();
    this->file_track()->drop_head_and_offset_at(current_frame);
}

void project_editor::drop_tail_and_offset() {
    if (!this->can_split()) {
        return;
    }

    auto const current_frame = this->_player->current_frame();
    this->file_track()->drop_tail_and_offset_at(current_frame);
}

bool project_editor::can_erase() const {
    auto const current_frame = this->_player->current_frame();
    return this->file_track()->module_at(current_frame).has_value();
}

void project_editor::erase_and_offset() {
    if (!this->can_erase()) {
        return;
    }

    auto const current_frame = this->_player->current_frame();
    this->file_track()->erase_and_offset_at(current_frame);
}

bool project_editor::can_insert_marker() const {
    auto const current_frame = this->_player->current_frame();
    return this->marker_pool()->markers().count(current_frame) == 0;
}

void project_editor::insert_marker() {
    if (!this->can_insert_marker()) {
        return;
    }

    auto const current_frame = this->_player->current_frame();
    this->marker_pool()->insert_marker(marker{.frame = current_frame});
}

bool project_editor::can_return_to_zero() const {
    return this->_player->current_frame() != 0;
}

void project_editor::return_to_zero() {
    if (!this->can_return_to_zero()) {
        return;
    }

    this->_player->seek(0);
}

bool project_editor::can_go_to_marker(std::size_t const marker_idx) const {
    auto const &marker_pool = this->marker_pool();
    if (auto const marker = marker_pool->marker_at(marker_idx)) {
        return this->_player->current_frame() != marker->frame;
    } else {
        return false;
    }
}

void project_editor::go_to_marker(std::size_t const marker_idx) {
    if (!this->can_go_to_marker(marker_idx)) {
        return;
    }

    auto const &marker_pool = this->marker_pool();
    if (auto const marker = marker_pool->marker_at(marker_idx)) {
        this->_player->seek(marker->frame);
    }
}

bool project_editor::can_undo() const {
    return this->database()->can_undo();
}

void project_editor::undo() {
    if (!this->can_undo()) {
        return;
    }

    this->database()->undo();
}

bool project_editor::can_redo() const {
    return this->database()->can_redo();
}

void project_editor::redo() {
    if (!this->can_redo()) {
        return;
    }

    this->database()->redo();
}

std::optional<proc::frame_index_t> project_editor::previous_edge() const {
    frame_index_t const current_frame = this->_player->current_frame();
    auto const file_track_edge = this->_file_track->previous_edge(current_frame);
    auto const marker_pool_edge = this->_marker_pool->previous_edge(current_frame);

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

std::optional<proc::frame_index_t> project_editor::next_edge() const {
    frame_index_t const current_frame = this->_player->current_frame();
    auto const file_track_edge = this->file_track()->next_edge(current_frame);
    auto const marker_pool_edge = this->marker_pool()->next_edge(current_frame);

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

std::map<proc::frame_index_t, marker> const &project_editor::markers() const {
    return this->_marker_pool->markers();
}

file_track_module_map_t const &project_editor::modules() const {
    return this->_file_track->modules();
}

observing::syncable project_editor::observe_is_playing(std::function<void(bool const &)> &&handler) {
    return this->_player->observe_is_playing(std::move(handler));
}

observing::syncable project_editor::observe_file_track_event(std::function<void(file_track_event const &)> &&handler) {
    return this->_file_track->observe_event(std::move(handler));
}

observing::syncable project_editor::observe_marker_pool_event(
    std::function<void(marker_pool_event const &)> &&handler) {
    return this->_marker_pool->observe_event(std::move(handler));
}

std::shared_ptr<project_editor> project_editor::make_shared(
    url const &editing_file_url, url const &db_file_url, ae::file_info const &file_info,
    std::shared_ptr<player_for_project_editor> const &player,
    std::shared_ptr<action_controller> const &action_controller) {
    return make_shared(editing_file_url, file_info, player, file_track::make_shared(), marker_pool::make_shared(),
                       database::make_shared(db_file_url), action_controller);
}

std::shared_ptr<project_editor> project_editor::make_shared(
    url const &editing_file_url, ae::file_info const &file_info,
    std::shared_ptr<player_for_project_editor> const &player,
    std::shared_ptr<file_track_for_project_editor> const &file_track,
    std::shared_ptr<marker_pool_for_project_editor> const &marker_pool,
    std::shared_ptr<database_for_project_editor> const &database,
    std::shared_ptr<action_controller> const &action_controller) {
    return std::shared_ptr<project_editor>(
        new project_editor{editing_file_url, file_info, player, file_track, marker_pool, database, action_controller});
}

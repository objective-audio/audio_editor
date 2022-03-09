//
//  ae_project_editor.cpp
//

#include "ae_project_editor.h"

#include <audio_editor_core/ae_database.h>
#include <audio_editor_core/ae_edge_editor.h>
#include <audio_editor_core/ae_exporter.h>
#include <audio_editor_core/ae_file_loader.h>
#include <audio_editor_core/ae_file_track.h>
#include <audio_editor_core/ae_hierarchy.h>
#include <audio_editor_core/ae_marker_pool.h>
#include <audio_editor_core/ae_pasteboard.h>
#include <audio_editor_core/ae_player.h>
#include <audio_editor_core/ae_project_editor_utils.h>
#include <audio_editor_core/ae_project_url.h>
#include <audio_editor_core/ae_responder_stack.h>
#include <audio_editor_core/ae_time_editor.h>
#include <audio_editor_core/ae_time_editor_level.h>
#include <audio_editor_core/ae_time_editor_level_pool.h>
#include <cpp_utils/yas_fast_each.h>
#include <processing/yas_processing_umbrella.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<project_editor> project_editor::make_shared(
    std::string const &identifier, ae::file_info const &file_info,
    std::shared_ptr<file_track_for_project_editor> const &file_track,
    std::shared_ptr<marker_pool_for_project_editor> const &marker_pool,
    std::shared_ptr<edge_editor_for_project_editor> const &edge_editor,
    std::shared_ptr<pasteboard_for_project_editor> const &pasteboard,
    std::shared_ptr<database_for_project_editor> const &database,
    std::shared_ptr<exporter_for_project_editor> const &exporter,
    std::shared_ptr<nudging_for_project_editor> const &nudging,
    std::shared_ptr<timing_for_project_editor> const &timing,
    std::shared_ptr<time_editor_level_pool> const &time_editor_level_pool) {
    auto const &project_level = hierarchy::project_level_for_id(identifier);
    auto const &project_url = project_level->project_url;
    return std::shared_ptr<project_editor>(new project_editor{
        project_url->editing_file(), file_info, project_level->player, file_track, marker_pool, edge_editor, pasteboard,
        database, exporter, project_level->action_controller, project_level->dialog_presenter, nudging, timing,
        project_level->responder_stack, time_editor_level_pool});
}

project_editor::project_editor(url const &editing_file_url, ae::file_info const &file_info,
                               std::shared_ptr<player_for_project_editor> const &player,
                               std::shared_ptr<file_track_for_project_editor> const &file_track,
                               std::shared_ptr<marker_pool_for_project_editor> const &marker_pool,
                               std::shared_ptr<edge_editor_for_project_editor> const &edge_editor,
                               std::shared_ptr<pasteboard_for_project_editor> const &pasteboard,
                               std::shared_ptr<database_for_project_editor> const &database,
                               std::shared_ptr<exporter_for_project_editor> const &exporter,
                               std::shared_ptr<action_controller> const &action_controller,
                               std::shared_ptr<dialog_presenter> const &dialog_presenter,
                               std::shared_ptr<nudging_for_project_editor> const &nudging,
                               std::shared_ptr<timing_for_project_editor> const &timing,
                               std::shared_ptr<responder_stack> const &responder_stack,
                               std::shared_ptr<time_editor_level_pool> const &time_editor_level_pool)
    : _editing_file_url(editing_file_url),
      _file_info(file_info),
      _player(player),
      _file_track(file_track),
      _timeline(proc::timeline::make_shared()),
      _track(proc::track::make_shared()),
      _marker_pool(marker_pool),
      _edge_editor(edge_editor),
      _pasteboard(pasteboard),
      _database(database),
      _exporter(exporter),
      _action_controller(action_controller),
      _dialog_presenter(dialog_presenter),
      _nudging(nudging),
      _timing(timing),
      _responder_stack(responder_stack),
      _time_editor_level_pool(time_editor_level_pool) {
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
                    }
                    break;
                case file_track_event_type::erased:
                    if (auto const &track = this->_track) {
                        auto const &file_module = event.module.value();
                        track->erase_modules_for_range(file_module.range);
                        this->_database->remove_module(file_module.range);
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
                    }
                    break;
                case marker_pool_event_type::reverted:
                    break;
                case marker_pool_event_type::inserted:
                    this->_database->add_marker(event.marker.value());
                    break;
                case marker_pool_event_type::erased:
                    this->_database->remove_marker(event.marker.value().frame);
                    break;
            }
        })
        .sync()
        ->add_to(this->_pool);

    this->_edge_editor
        ->observe_event([this](edge_editor_event const &event) {
            switch (event.type) {
                case edge_editor_event_type::updated:
                    this->_database->set_edge(event.edge);
                    break;

                case edge_editor_event_type::fetched:
                case edge_editor_event_type::reverted:
                    break;
            }
        })
        .sync()
        ->add_to(this->_pool);

    this->_pasteboard
        ->observe_event([this](pasteboard_event const &event) {
            switch (event) {
                case pasteboard_event::file_module:
                    this->_database->set_pasting_data(this->_pasteboard->data());
                    break;
                case pasteboard_event::fetched:
                case pasteboard_event::reverted:
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

            if (auto const &db_edge = this->_database->edge()) {
                this->_edge_editor->revert_edge(db_edge.value().edge());
            } else {
                this->_edge_editor->revert_edge(ae::edge::zero());
            }

            this->_pasteboard->revert_data(this->_database->pasting_data());
        })
        .end()
        ->add_to(this->_pool);

    // プロジェクトの初期状態を作る。事前にdbに直接挿入してrevertから始めるべきかもしれない。
    this->_database->suspend_saving([this, &file_info] {
        this->_file_track->insert_module_and_notify(
            file_module{.range = time::range{0, file_info.length}, .file_frame = 0});

        this->_edge_editor->set_edge({.begin_frame = 0, .end_frame = static_cast<frame_index_t>(file_info.length)});
    });

    this->_player->begin_rendering();
}

bool project_editor::can_nudge() const {
    return !this->_player->is_playing();
}

void project_editor::nudge_previous(uint32_t const offset_count) {
    if (!this->can_nudge()) {
        return;
    }

    frame_index_t const current_frame = this->_player->current_frame();
    frame_index_t const prev_frame = this->_nudging->previous_frame(current_frame, offset_count);
    this->_player->seek(prev_frame);
}

void project_editor::nudge_next(uint32_t const offset_count) {
    if (!this->can_nudge()) {
        return;
    }

    frame_index_t const current_frame = this->_player->current_frame();
    frame_index_t const next_frame = this->_nudging->next_frame(current_frame, offset_count);
    this->_player->seek(next_frame);
}

void project_editor::rotate_timing_fraction() {
    switch (this->_timing->fraction_kind()) {
        case timing_fraction_kind::sample:
            this->_timing->set_fraction_kind(timing_fraction_kind::milisecond);
            break;
        case timing_fraction_kind::milisecond:
            this->_timing->set_fraction_kind(timing_fraction_kind::frame30);
            break;
        case timing_fraction_kind::frame30:
            this->_timing->set_fraction_kind(timing_fraction_kind::sample);
            break;
    }
}

bool project_editor::can_jump_to_previous_edge() const {
    return this->_previous_jumpable_frame().has_value();
}

bool project_editor::can_jump_to_next_edge() const {
    return this->_next_jumpable_frame().has_value();
}

bool project_editor::can_jump_to_beginnig() const {
    if (auto const edge = this->_first_edge().has_value()) {
        if (edge != this->_player->current_frame()) {
            return true;
        }
    }
    return false;
}

bool project_editor::can_jump_to_end() const {
    if (auto const edge = this->_last_edge().has_value()) {
        if (edge != this->_player->current_frame()) {
            return true;
        }
    }
    return false;
}

void project_editor::jump_to_previous_edge() {
    if (auto const edge = this->_previous_jumpable_frame()) {
        this->_player->seek(edge.value());
    }
}

void project_editor::jump_to_next_edge() {
    if (auto const edge = this->_next_jumpable_frame()) {
        this->_player->seek(edge.value());
    }
}

void project_editor::jump_to_beginning() {
    if (auto const edge = this->_first_edge()) {
        this->_player->seek(edge.value());
    }
}

void project_editor::jump_to_end() {
    if (auto const edge = this->_last_edge()) {
        this->_player->seek(edge.value());
    }
}

bool project_editor::can_split() const {
    if (!this->_can_editing()) {
        return false;
    }

    auto const &file_track = this->_file_track;
    auto const current_frame = this->_player->current_frame();
    return file_track->splittable_module_at(current_frame).has_value();
}

void project_editor::split() {
    if (!this->can_split()) {
        return;
    }

    this->_database->suspend_saving([this] {
        auto const current_frame = this->_player->current_frame();
        this->_file_track->split_at(current_frame);
    });
}

void project_editor::drop_head() {
    if (!this->can_split()) {
        return;
    }

    this->_database->suspend_saving([this] {
        auto const current_frame = this->_player->current_frame();
        this->_file_track->drop_head_at(current_frame);
    });
}

void project_editor::drop_tail() {
    if (!this->can_split()) {
        return;
    }

    this->_database->suspend_saving([this] {
        auto const current_frame = this->_player->current_frame();
        this->_file_track->drop_tail_at(current_frame);
    });
}

void project_editor::drop_head_and_offset() {
    if (!this->can_split()) {
        return;
    }

    auto const current_frame = this->_player->current_frame();
    auto const seek_frame = this->_file_track->module_at(current_frame).value().range.frame;

    this->_database->suspend_saving([this, &current_frame] {
        auto const module_range = this->_file_track->module_at(current_frame)->range;
        this->_file_track->drop_head_and_offset_at(current_frame);
        auto const dropping_length = current_frame - module_range.frame;
        this->_marker_pool->erase_range({module_range.frame, static_cast<proc::length_t>(dropping_length)});
        auto const offset = -dropping_length;
        this->_marker_pool->move_offset_from(current_frame, offset);
    });

    this->_player->seek(seek_frame);
}

void project_editor::drop_tail_and_offset() {
    if (!this->can_split()) {
        return;
    }

    this->_database->suspend_saving([this] {
        auto const current_frame = this->_player->current_frame();
        auto const module_range = this->_file_track->module_at(current_frame)->range;
        this->_file_track->drop_tail_and_offset_at(current_frame);
        auto const dropping_length = module_range.next_frame() - current_frame;
        this->_marker_pool->erase_range({current_frame, static_cast<proc::length_t>(dropping_length)});
        auto const offset = -dropping_length;
        this->_marker_pool->move_offset_from(module_range.next_frame(), offset);
    });
}

bool project_editor::can_erase() const {
    if (!this->_can_editing()) {
        return false;
    }

    auto const current_frame = this->_player->current_frame();
    return this->_file_track->module_at(current_frame).has_value();
}

void project_editor::erase() {
    if (!this->can_erase()) {
        return;
    }

    this->_database->suspend_saving([this] {
        auto const current_frame = this->_player->current_frame();
        this->_file_track->erase_at(current_frame);
    });
}

void project_editor::erase_and_offset() {
    if (!this->can_erase()) {
        return;
    }

    auto const current_frame = this->_player->current_frame();
    auto const previous_module = this->_file_track->previous_module_at(current_frame);

    this->_database->suspend_saving([this] {
        auto const current_frame = this->_player->current_frame();
        auto const erasing_range = this->_file_track->module_at(current_frame)->range;
        this->_file_track->erase_and_offset_at(current_frame);
        this->_marker_pool->erase_range(erasing_range);
        auto const offset = -static_cast<frame_index_t>(erasing_range.length);
        this->_marker_pool->move_offset_from(erasing_range.next_frame(), offset);
    });

    if (auto const &module = previous_module) {
        this->_player->seek(module->range.next_frame());
    }
}

bool project_editor::can_insert_marker() const {
    if (!this->_can_editing()) {
        return false;
    }

    auto const current_frame = this->_player->current_frame();
    return this->_marker_pool->markers().count(current_frame) == 0;
}

void project_editor::insert_marker() {
    if (!this->can_insert_marker()) {
        return;
    }

    this->_database->suspend_saving([this] {
        auto const current_frame = this->_player->current_frame();
        this->_marker_pool->insert_marker(marker{.frame = current_frame});
    });
}

bool project_editor::can_set_begin_edge() const {
    if (!this->_can_editing()) {
        return false;
    }

    auto const current_frame = this->_player->current_frame();
    auto const begin_frame = this->_edge_editor->edge().begin_frame;

    return current_frame != begin_frame;
}

bool project_editor::can_set_end_edge() const {
    if (!this->_can_editing()) {
        return false;
    }

    auto const current_frame = this->_player->current_frame();
    auto const end_frame = this->_edge_editor->edge().end_frame;

    return current_frame != end_frame;
}

void project_editor::set_begin_edge() {
    if (!this->can_set_begin_edge()) {
        return;
    }

    auto const current_frame = this->_player->current_frame();
    this->_edge_editor->set_begin_frame(current_frame);
}

void project_editor::set_end_edge() {
    if (!this->can_set_end_edge()) {
        return;
    }

    auto const current_frame = this->_player->current_frame();
    this->_edge_editor->set_end_frame(current_frame);
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
    auto const &marker_pool = this->_marker_pool;
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

    auto const &marker_pool = this->_marker_pool;
    if (auto const marker = marker_pool->marker_at(marker_idx)) {
        this->_player->seek(marker->frame);
    }
}

bool project_editor::can_undo() const {
    if (!this->_can_editing()) {
        return false;
    }

    return this->_database->can_undo();
}

void project_editor::undo() {
    if (!this->can_undo()) {
        return;
    }

    this->_database->undo();
}

bool project_editor::can_redo() const {
    if (!this->_can_editing()) {
        return false;
    }

    return this->_database->can_redo();
}

void project_editor::redo() {
    if (!this->can_redo()) {
        return;
    }

    this->_database->redo();
}

bool project_editor::can_select_file_for_export() const {
    return this->can_export_to_file();
}

void project_editor::select_file_for_export() {
    if (!this->can_select_file_for_export()) {
        return;
    }

    this->_dialog_presenter->notify_event(dialog_event::select_file_for_export);
}

bool project_editor::can_export_to_file() const {
    if (!this->_can_editing()) {
        return false;
    }

    return this->_edge_editor->edge().range().has_value();
}

void project_editor::export_to_file(url const &export_url) {
    if (!this->can_export_to_file()) {
        return;
    }

    auto const range = this->_edge_editor->edge().range();
    if (!range.has_value()) {
        return;
    }

    this->_player->set_playing(false);

    exporting_format const format{.sample_rate = this->_file_info.sample_rate,
                                  .pcm_format = audio::pcm_format::float32,
                                  .channel_count = this->_file_info.channel_count};

    this->_exporter->begin(export_url, this->_timeline, format, range.value());
}

bool project_editor::can_cut() const {
    return this->can_copy();
}

void project_editor::cut_and_offset() {
    if (!this->can_cut()) {
        return;
    }

    this->_database->suspend_saving([this] {
        this->copy();

        auto const current_frame = this->_player->current_frame();
        auto const erasing_range = this->_file_track->module_at(current_frame)->range;
        this->_file_track->erase_and_offset_at(current_frame);
        this->_marker_pool->erase_range(erasing_range);
        auto const offset = -static_cast<frame_index_t>(erasing_range.length);
        this->_marker_pool->move_offset_from(erasing_range.next_frame(), offset);
    });
}

bool project_editor::can_copy() const {
    if (!this->_can_editing()) {
        return false;
    }

    auto const &file_track = this->_file_track;
    auto const current_frame = this->_player->current_frame();
    return file_track->module_at(current_frame).has_value();
}

void project_editor::copy() {
    if (!this->can_copy()) {
        return;
    }

    this->_database->suspend_saving([this] {
        auto const &file_track = this->_file_track;
        auto const current_frame = this->_player->current_frame();
        if (auto const file_module = file_track->module_at(current_frame)) {
            auto const &value = file_module.value();
            this->_pasteboard->set_file_module({.file_frame = value.file_frame, .length = value.range.length});
        }
    });
}

bool project_editor::can_paste() const {
    if (!this->_can_editing()) {
        return false;
    }

    if (!this->_pasteboard->file_module().has_value()) {
        return false;
    }

    auto const &file_track = this->_file_track;
    auto const current_frame = this->_player->current_frame();

    if (file_track->modules().empty()) {
        // moduleが何もなければペーストできる
        return true;
    } else if (file_track->module_at(current_frame).has_value()) {
        // 今いるframeの場所にmoduleがあればペーストできる
        return true;
    } else if (file_track->module_at(current_frame - 1).has_value()) {
        // 今あるmoduleのピッタリ後ろならペーストできる
        return true;
    }

    return false;
}

void project_editor::paste_and_offset() {
    if (!this->can_paste()) {
        return;
    }

    if (auto const module = this->_pasteboard->file_module()) {
        this->_database->suspend_saving([this, &module] {
            auto const module_value = module.value();
            auto const current_frame = this->_player->current_frame();

            this->_file_track->split_and_insert_module_and_offset(
                {.file_frame = module_value.file_frame, .range = {current_frame, module_value.length}});

            this->_marker_pool->move_offset_from(current_frame, module_value.length);
        });
    }
}

bool project_editor::can_begin_time_editing() const {
    return this->_time_editor_level_pool->level() == nullptr;
}

bool project_editor::can_end_time_editing() const {
    return this->_time_editor_level_pool->level() != nullptr;
}

bool project_editor::can_input_time_number() const {
    return this->_time_editor_level_pool->level() != nullptr;
}

bool project_editor::can_select_time_unit() const {
    return this->can_begin_time_editing();
}

void project_editor::begin_time_editing(std::optional<std::size_t> const unit_idx) {
    if (!this->can_begin_time_editing()) {
        return;
    }

    auto const current_frame = this->_player->current_frame();
    auto const components = this->_timing->components(current_frame);

    this->_time_editor_level_pool->add_level(components.raw_components(), unit_idx);

    auto const &time_editor = this->_time_editor_level_pool->level()->time_editor;

    if (auto const responder_stack = this->_responder_stack.lock()) {
        responder_stack->push_responder(time_editor);
    }

    time_editor
        ->observe_event([this](time_editor_event const &) {
            if (auto const components = this->_time_editor_level_pool->level()->time_editor->finalized_components()) {
                auto const frame = this->_timing->frame(timing_components{components.value()});
                this->_player->seek(frame);
            }

            if (auto const &level = this->_time_editor_level_pool->level()) {
                if (auto const responder_stack = this->_responder_stack.lock()) {
                    responder_stack->pop_responder(level->time_editor->responder_id());
                }
            }

            this->_time_editor_level_pool->remove_level();
            this->_time_editing_canceller->cancel();
            this->_time_editing_canceller = nullptr;
        })
        .end()
        ->set_to(this->_time_editing_canceller);
}

void project_editor::finish_time_editing() {
    if (auto const &level = this->_time_editor_level_pool->level()) {
        level->time_editor->finish();
    }
}

void project_editor::cancel_time_editing() {
    if (auto const &level = this->_time_editor_level_pool->level()) {
        level->time_editor->cancel();
    }
}

void project_editor::input_time_number(uint32_t const number) {
    if (auto const &level = this->_time_editor_level_pool->level()) {
        level->time_editor->input_number(number);
    }
}

void project_editor::delete_time_number() {
    if (auto const &level = this->_time_editor_level_pool->level()) {
        level->time_editor->delete_number();
    }
}

void project_editor::increment_time_number() {
    if (auto const &level = this->_time_editor_level_pool->level()) {
        level->time_editor->increment_number();
    }
}

void project_editor::decrement_time_number() {
    if (auto const &level = this->_time_editor_level_pool->level()) {
        level->time_editor->decrement_number();
    }
}

void project_editor::move_to_previous_time_unit() {
    if (auto const &level = this->_time_editor_level_pool->level()) {
        level->time_editor->move_to_previous_unit();
    }
}

void project_editor::move_to_next_time_unit() {
    if (auto const &level = this->_time_editor_level_pool->level()) {
        level->time_editor->move_to_next_unit();
    }
}

void project_editor::change_time_sign_to_plus() {
    if (auto const &level = this->_time_editor_level_pool->level()) {
        level->time_editor->change_sign_to_plus();
    }
}

void project_editor::change_time_sign_to_minus() {
    if (auto const &level = this->_time_editor_level_pool->level()) {
        level->time_editor->change_sign_to_minus();
    }
}

void project_editor::select_time_unit(std::size_t const unit_idx) {
    this->begin_time_editing(unit_idx);

    if (auto const &level = this->_time_editor_level_pool->level()) {
        level->time_editor->set_unit_idx(unit_idx);
    }
}

#pragma mark - responder

identifier project_editor::responder_id() {
    return this->_responder_id;
}

std::optional<ae::action> project_editor::to_action(ae::key const &key) {
    switch (key) {
        case key::space:
            return action_kind::toggle_play;
        case key::del:
            return action_kind::erase;
        case key::a:
            return action_kind::drop_head;
        case key::s:
            return action_kind::split;
        case key::d:
            return action_kind::drop_tail;
        case key::n:
            return action_kind::rotate_nudging_next_unit;
        case key::shift_n:
            return action_kind::rotate_nudging_previous_unit;
        case key::f:
            return action_kind::rotate_timing_fraction;
        case key::t:
            return action_kind::begin_time_editing;
        case key::left_bracket:
            return action_kind::set_begin_edge;
        case key::right_bracket:
            return action_kind::set_end_edge;
        case key::left:
            return action_kind::nudge_previous;
        case key::right:
            return action_kind::nudge_next;
        case key::shift_left:
            return action_kind::nudge_previous_more;
        case key::shift_right:
            return action_kind::nudge_next_more;
        case key::num_0:
            return action_kind::return_to_zero;
        case key::num_1:
            return action{action_kind::go_to_marker, "0"};
        case key::num_2:
            return action{action_kind::go_to_marker, "1"};
        case key::num_3:
            return action{action_kind::go_to_marker, "2"};
        case key::num_4:
            return action{action_kind::go_to_marker, "3"};
        case key::num_5:
            return action{action_kind::go_to_marker, "4"};
        case key::num_6:
            return action{action_kind::go_to_marker, "5"};
        case key::num_7:
            return action{action_kind::go_to_marker, "6"};
        case key::num_8:
            return action{action_kind::go_to_marker, "7"};
        case key::num_9:
            return action{action_kind::go_to_marker, "8"};

        case key::esc:
        case key::plus:
        case key::hyphen:
        case key::ret:
        case key::tab:
        case key::shift_tab:
        case key::up:
        case key::down:
            return std::nullopt;
    }
}

void project_editor::handle_action(ae::action const &action) {
    auto const responding = this->responding_to_action(action);
    switch (responding) {
        case responding::accepting: {
            switch (action.kind) {
                case action_kind::toggle_play:
                    this->_player->set_playing(!this->_player->is_playing());
                    break;
                case action_kind::nudge_previous:
                    this->nudge_previous(1);
                    break;
                case action_kind::nudge_next:
                    this->nudge_next(1);
                    break;
                case action_kind::nudge_previous_more:
                    this->nudge_previous(10);
                    break;
                case action_kind::nudge_next_more:
                    this->nudge_next(10);
                    break;
                case action_kind::rotate_nudging_next_unit:
                    this->_nudging->rotate_next_unit();
                    break;
                case action_kind::rotate_nudging_previous_unit:
                    this->_nudging->rotate_previous_unit();
                    break;
                case action_kind::rotate_timing_fraction:
                    this->rotate_timing_fraction();
                    break;
                case action_kind::jump_previous:
                    this->jump_to_previous_edge();
                    break;
                case action_kind::jump_next:
                    this->jump_to_next_edge();
                    break;
                case action_kind::jump_to_beginning:
                    this->jump_to_beginning();
                    break;
                case action_kind::jump_to_end:
                    this->jump_to_end();
                    break;
                case action_kind::drop_head:
                    this->drop_head();
                    break;
                case action_kind::split:
                    this->split();
                    break;
                case action_kind::drop_tail:
                    this->drop_tail();
                    break;
                case action_kind::erase:
                    this->erase();
                    break;
                case action_kind::insert_marker:
                    this->insert_marker();
                    break;
                case action_kind::set_begin_edge:
                    this->set_begin_edge();
                    break;
                case action_kind::set_end_edge:
                    this->set_end_edge();
                    break;
                case action_kind::return_to_zero:
                    this->return_to_zero();
                    break;
                case action_kind::go_to_marker:
                    this->go_to_marker(std::stoi(action.value));
                    break;
                case action_kind::undo:
                    this->undo();
                    break;
                case action_kind::redo:
                    this->redo();
                    break;
                case action_kind::select_file_for_export:
                    this->select_file_for_export();
                    break;
                case action_kind::export_to_file:
                    this->export_to_file(url::file_url(action.value));
                    break;
                case action_kind::cut:
                    this->cut_and_offset();
                    break;
                case action_kind::copy:
                    this->copy();
                    break;
                case action_kind::paste:
                    this->paste_and_offset();
                    break;

                case action_kind::begin_time_editing:
                    this->begin_time_editing(std::nullopt);
                    break;
                case action_kind::select_time_unit:
                    this->select_time_unit(std::stoi(action.value));
                    break;

                    // 以下、time_editor用
                case action_kind::finish_time_editing:
                case action_kind::cancel_time_editing:
                case action_kind::move_to_previous_time_unit:
                case action_kind::move_to_next_time_unit:
                case action_kind::input_time:
                case action_kind::delete_time:
                case action_kind::increment_time:
                case action_kind::decrement_time:
                case action_kind::change_time_sign_to_plus:
                case action_kind::change_time_sign_to_minus:
                    break;
            }
        } break;

        case responding::blocking:
        case responding::fallthrough:
            break;
    }
}

responding project_editor::responding_to_action(ae::action const &action) {
    auto const to_responding = [](bool const &flag) { return flag ? responding::accepting : responding::blocking; };

    switch (action.kind) {
        case action_kind::toggle_play:
            return responding::accepting;
        case action_kind::nudge_previous:
        case action_kind::nudge_next:
        case action_kind::nudge_previous_more:
        case action_kind::nudge_next_more:
            return to_responding(this->can_nudge());
        case action_kind::rotate_nudging_next_unit:
        case action_kind::rotate_nudging_previous_unit:
            return responding::accepting;
        case action_kind::rotate_timing_fraction:
            return responding::accepting;

        case action_kind::jump_previous:
            return to_responding(this->can_jump_to_previous_edge());
        case action_kind::jump_next:
            return to_responding(this->can_jump_to_next_edge());
        case action_kind::jump_to_beginning:
            return to_responding(this->can_jump_to_beginnig());
        case action_kind::jump_to_end:
            return to_responding(this->can_jump_to_end());

        case action_kind::drop_head:
            return to_responding(this->can_split());
        case action_kind::split:
            return to_responding(this->can_split());
        case action_kind::drop_tail:
            return to_responding(this->can_split());

        case action_kind::erase:
            return to_responding(this->can_erase());

        case action_kind::insert_marker:
            return to_responding(this->can_insert_marker());

        case action_kind::return_to_zero:
            return to_responding(this->can_return_to_zero());
        case action_kind::go_to_marker:
            return to_responding(this->can_go_to_marker(std::stoi(action.value)));

        case action_kind::undo:
            return to_responding(this->can_undo());
        case action_kind::redo:
            return to_responding(this->can_redo());

        case action_kind::select_file_for_export:
            return to_responding(this->can_select_file_for_export());
        case action_kind::export_to_file:
            return to_responding(this->can_export_to_file());

        case action_kind::cut:
            return to_responding(this->can_cut());
        case action_kind::copy:
            return to_responding(this->can_copy());
        case action_kind::paste:
            return to_responding(this->can_paste());

        case action_kind::cancel_time_editing:
            return to_responding(this->can_end_time_editing());
        case action_kind::begin_time_editing:
            return to_responding(this->can_begin_time_editing());
        case action_kind::select_time_unit:
            return to_responding(this->can_select_time_unit());

        case action_kind::set_begin_edge:
            return to_responding(this->can_set_begin_edge());
        case action_kind::set_end_edge:
            return to_responding(this->can_set_end_edge());

            // 以下、time_editor用
        case action_kind::finish_time_editing:
        case action_kind::move_to_previous_time_unit:
        case action_kind::move_to_next_time_unit:
        case action_kind::input_time:
        case action_kind::delete_time:
        case action_kind::increment_time:
        case action_kind::decrement_time:
        case action_kind::change_time_sign_to_plus:
        case action_kind::change_time_sign_to_minus:
            return responding::fallthrough;
    }
}

#pragma mark - private

std::optional<frame_index_t> project_editor::_previous_jumpable_frame() const {
    frame_index_t const current_frame = this->_player->current_frame();

    std::optional<frame_index_t> result{std::nullopt};

    std::initializer_list<std::shared_ptr<jumpable_on_project_editor>> const editors{
        this->_file_track, this->_marker_pool, this->_edge_editor};

    for (auto const &editor : editors) {
        if (auto const frame = editor->previous_jumpable_frame(current_frame)) {
            if (result.has_value()) {
                result = std::max(result.value(), frame.value());
            } else {
                result = frame.value();
            }
        }
    }

    return result;
}

std::optional<frame_index_t> project_editor::_next_jumpable_frame() const {
    frame_index_t const current_frame = this->_player->current_frame();

    std::optional<frame_index_t> result{std::nullopt};

    std::initializer_list<std::shared_ptr<jumpable_on_project_editor>> const editors{
        this->_file_track, this->_marker_pool, this->_edge_editor};

    for (auto const &editor : editors) {
        if (auto const frame = editor->next_jumpable_frame(current_frame)) {
            if (result.has_value()) {
                result = std::min(result.value(), frame.value());
            } else {
                result = frame.value();
            }
        }
    }

    return result;
}

std::optional<frame_index_t> project_editor::_first_edge() const {
    if (auto const module = this->_file_track->first_module()) {
        return module.value().range.frame;
    } else {
        return std::nullopt;
    }
}

std::optional<frame_index_t> project_editor::_last_edge() const {
    if (auto const module = this->_file_track->last_module()) {
        return module.value().range.next_frame();
    } else {
        return std::nullopt;
    }
}

bool project_editor::_can_editing() const {
    return !this->_exporter->is_exporting();
}

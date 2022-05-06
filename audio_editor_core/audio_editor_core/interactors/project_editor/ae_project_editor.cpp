//
//  ae_project_editor.cpp
//

#include "ae_project_editor.h"

#include <audio_editor_core/ae_database.h>
#include <audio_editor_core/ae_dialog_presenter.h>
#include <audio_editor_core/ae_edge_editor.h>
#include <audio_editor_core/ae_exporter.h>
#include <audio_editor_core/ae_file_loader.h>
#include <audio_editor_core/ae_file_track.h>
#include <audio_editor_core/ae_hierarchy.h>
#include <audio_editor_core/ae_marker_pool.h>
#include <audio_editor_core/ae_pasteboard.h>
#include <audio_editor_core/ae_player.h>
#include <audio_editor_core/ae_project_editor_utils.h>
#include <audio_editor_core/ae_responder_stack.h>
#include <audio_editor_core/ae_sheet_presenter.h>
#include <audio_editor_core/ae_time_editor.h>
#include <audio_editor_core/ae_time_editor_closer.h>
#include <audio_editor_core/ae_time_editor_level.h>
#include <audio_editor_core/ae_time_editor_level_router.h>
#include <audio_editor_core/ae_time_editor_responder.h>
#include <audio_editor_core/ae_timeline_updater.h>
#include <cpp_utils/yas_fast_each.h>
#include <processing/yas_processing_umbrella.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<project_editor> project_editor::make_shared(
    std::string const &project_id, ae::file_info const &file_info,
    std::shared_ptr<file_track_for_project_editor> const &file_track,
    std::shared_ptr<marker_pool_for_project_editor> const &marker_pool,
    std::shared_ptr<edge_editor_for_project_editor> const &edge_editor,
    std::shared_ptr<pasteboard_for_project_editor> const &pasteboard,
    std::shared_ptr<database_for_project_editor> const &database,
    std::shared_ptr<exporter_for_project_editor> const &exporter,
    std::shared_ptr<timing_for_project_editor> const &timing,
    std::shared_ptr<time_editor_level_router> const &time_editor_level_router,
    std::shared_ptr<timeline_updater> const &timeline_updater) {
    auto const &project_level = hierarchy::project_level_for_id(project_id);
    return std::shared_ptr<project_editor>(
        new project_editor{file_info, project_level->player, file_track, marker_pool, edge_editor, pasteboard, database,
                           exporter, project_level->dialog_presenter, project_level->sheet_presenter, timing,
                           project_level->responder_stack, time_editor_level_router, timeline_updater});
}

project_editor::project_editor(ae::file_info const &file_info, std::shared_ptr<player_for_project_editor> const &player,
                               std::shared_ptr<file_track_for_project_editor> const &file_track,
                               std::shared_ptr<marker_pool_for_project_editor> const &marker_pool,
                               std::shared_ptr<edge_editor_for_project_editor> const &edge_editor,
                               std::shared_ptr<pasteboard_for_project_editor> const &pasteboard,
                               std::shared_ptr<database_for_project_editor> const &database,
                               std::shared_ptr<exporter_for_project_editor> const &exporter,
                               std::shared_ptr<dialog_presenter> const &dialog_presenter,
                               std::shared_ptr<sheet_presenter> const &sheet_presenter,
                               std::shared_ptr<timing_for_project_editor> const &timing,
                               std::shared_ptr<responder_stack> const &responder_stack,
                               std::shared_ptr<time_editor_level_router> const &time_editor_level_router,
                               std::shared_ptr<timeline_updater> const &timeline_updater)
    : _file_info(file_info),
      _player(player),
      _file_track(file_track),
      _marker_pool(marker_pool),
      _edge_editor(edge_editor),
      _pasteboard(pasteboard),
      _database(database),
      _exporter(exporter),
      _dialog_presenter(dialog_presenter),
      _sheet_presenter(sheet_presenter),
      _timing(timing),
      _responder_stack(responder_stack),
      _time_editor_level_router(time_editor_level_router),
      _timeline_updater(timeline_updater) {
    this->_file_track
        ->observe_event([this](file_track_event const &event) {
            switch (event.type) {
                case file_track_event_type::any: {
                    this->_timeline_updater->replace(event.modules);

                    for (auto const &pair : event.modules) {
                        auto const &file_module = pair.second;
                        this->_database->add_module(file_module);
                    }
                } break;
                case file_track_event_type::reverted: {
                    this->_timeline_updater->replace(event.modules);
                } break;
                case file_track_event_type::inserted: {
                    auto const &file_module = event.module.value();
                    this->_timeline_updater->insert(file_module);
                    this->_database->add_module(file_module);
                } break;
                case file_track_event_type::erased: {
                    auto const &range = event.module.value().range;
                    this->_timeline_updater->erase(range);
                    this->_database->remove_module(range);
                } break;
                case file_track_event_type::detail_updated: {
                    auto const &file_module = event.module.value();
                    this->_database->update_module_detail(file_module);
                } break;
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

    this->_exporter->begin(export_url, this->_timeline_updater->timeline(), format, range.value());
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

bool project_editor::can_begin_module_renaming() const {
    if (!this->_can_editing()) {
        return false;
    }

    return true;
}

void project_editor::begin_module_renaming(std::string const &range) {
    if (!this->can_begin_module_renaming()) {
        return;
    }

    this->_sheet_presenter->notify_event({.kind = sheet_kind::module_name, .value = range});
}

bool project_editor::can_begin_time_editing() const {
    return this->_time_editor_level_router->level() == nullptr;
}

bool project_editor::can_end_time_editing() const {
    return this->_time_editor_level_router->level() != nullptr;
}

bool project_editor::can_input_time_number() const {
    return this->_time_editor_level_router->level() != nullptr;
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

    this->_time_editor_level_router->add_level(components.raw_components(), unit_idx);

    auto const &level = this->_time_editor_level_router->level();

    if (auto const responder_stack = this->_responder_stack.lock()) {
        responder_stack->push_responder(level->instance_id, level->responder);
    }
}

void project_editor::finish_time_editing() {
    if (auto const &level = this->_time_editor_level_router->level()) {
        level->closer->finish();
    }
}

void project_editor::cancel_time_editing() {
    if (auto const &level = this->_time_editor_level_router->level()) {
        level->closer->cancel();
    }
}

void project_editor::input_time_number(uint32_t const number) {
    if (auto const &level = this->_time_editor_level_router->level()) {
        level->editor->input_number(number);
    }
}

void project_editor::delete_time_number() {
    if (auto const &level = this->_time_editor_level_router->level()) {
        level->editor->delete_number();
    }
}

void project_editor::increment_time_number() {
    if (auto const &level = this->_time_editor_level_router->level()) {
        level->editor->increment_number();
    }
}

void project_editor::decrement_time_number() {
    if (auto const &level = this->_time_editor_level_router->level()) {
        level->editor->decrement_number();
    }
}

void project_editor::move_to_previous_time_unit() {
    if (auto const &level = this->_time_editor_level_router->level()) {
        level->editor->move_to_previous_unit();
    }
}

void project_editor::move_to_next_time_unit() {
    if (auto const &level = this->_time_editor_level_router->level()) {
        level->editor->move_to_next_unit();
    }
}

void project_editor::change_time_sign_to_plus() {
    if (auto const &level = this->_time_editor_level_router->level()) {
        level->editor->change_sign_to_plus();
    }
}

void project_editor::change_time_sign_to_minus() {
    if (auto const &level = this->_time_editor_level_router->level()) {
        level->editor->change_sign_to_minus();
    }
}

void project_editor::select_time_unit(std::size_t const unit_idx) {
    this->begin_time_editing(unit_idx);

    if (auto const &level = this->_time_editor_level_router->level()) {
        level->editor->set_unit_idx(unit_idx);
    }
}

#pragma mark - private

bool project_editor::_can_editing() const {
    return !this->_exporter->is_exporting();
}

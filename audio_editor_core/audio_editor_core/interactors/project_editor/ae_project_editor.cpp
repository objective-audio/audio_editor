//
//  ae_project_editor.cpp
//

#include "ae_project_editor.h"

#include <audio_editor_core/ae_database.h>
#include <audio_editor_core/ae_edge_holder.h>
#include <audio_editor_core/ae_editing_status.h>
#include <audio_editor_core/ae_file_loader.h>
#include <audio_editor_core/ae_file_track.h>
#include <audio_editor_core/ae_hierarchy.h>
#include <audio_editor_core/ae_marker_pool.h>
#include <audio_editor_core/ae_pasteboard.h>
#include <audio_editor_core/ae_player.h>
#include <audio_editor_core/ae_project_editor_utils.h>
#include <audio_editor_core/ae_responder_stack.h>
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
    project_id const &project_id, ae::file_info const &file_info, file_track_for_project_editor *file_track,
    marker_pool_for_project_editor *marker_pool, edge_holder_for_project_editor *edge_holder,
    pasteboard_for_project_editor *pasteboard, database_for_project_editor *database,
    timeline_updater *timeline_updater, editing_status const *editing_status) {
    auto const &project_level = hierarchy::project_level_for_id(project_id);
    return std::shared_ptr<project_editor>(new project_editor{file_info, project_level->player.get(), file_track,
                                                              marker_pool, edge_holder, pasteboard, database,
                                                              timeline_updater, editing_status});
}

project_editor::project_editor(ae::file_info const &file_info, player_for_project_editor *player,
                               file_track_for_project_editor *file_track, marker_pool_for_project_editor *marker_pool,
                               edge_holder_for_project_editor *edge_holder, pasteboard_for_project_editor *pasteboard,
                               database_for_project_editor *database, timeline_updater *timeline_updater,
                               editing_status const *editing_status)
    : _file_info(file_info),
      _player(player),
      _file_track(file_track),
      _marker_pool(marker_pool),
      _edge_holder(edge_holder),
      _pasteboard(pasteboard),
      _database(database),
      _timeline_updater(timeline_updater),
      _editing_status(editing_status) {
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

    this->_edge_holder
        ->observe_event([this](edge_holder_event const &event) {
            switch (event.type) {
                case edge_holder_event_type::updated:
                    this->_database->set_edge(event.edge);
                    break;

                case edge_holder_event_type::fetched:
                case edge_holder_event_type::reverted:
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
                this->_edge_holder->revert_edge(db_edge.value().edge());
            } else {
                this->_edge_holder->revert_edge(ae::edge::zero());
            }

            this->_pasteboard->revert_data(this->_database->pasting_data());
        })
        .end()
        ->add_to(this->_pool);
}

bool project_editor::can_split() const {
    if (!this->_editing_status->can_editing()) {
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
    if (!this->_editing_status->can_editing()) {
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

bool project_editor::can_undo() const {
    if (!this->_editing_status->can_editing()) {
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
    if (!this->_editing_status->can_editing()) {
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
    if (!this->_editing_status->can_editing()) {
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
    if (!this->_editing_status->can_editing()) {
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

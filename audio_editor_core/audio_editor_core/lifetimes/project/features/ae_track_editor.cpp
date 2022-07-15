//
//  ae_track_editor.cpp
//

#include "ae_track_editor.h"

#include <audio_editor_core/ae_database.h>
#include <audio_editor_core/ae_edge_holder.h>
#include <audio_editor_core/ae_editing_status.h>
#include <audio_editor_core/ae_file_info_loader.h>
#include <audio_editor_core/ae_file_track.h>
#include <audio_editor_core/ae_hierarchy.h>
#include <audio_editor_core/ae_marker_pool.h>
#include <audio_editor_core/ae_pasteboard.h>
#include <audio_editor_core/ae_player.h>
#include <audio_editor_core/ae_time_editor.h>
#include <audio_editor_core/ae_time_editor_closer.h>
#include <audio_editor_core/ae_time_editor_lifetime.h>
#include <audio_editor_core/ae_timeline_holder_utils.h>
#include <cpp_utils/yas_fast_each.h>
#include <processing/yas_processing_umbrella.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<track_editor> track_editor::make_shared(player *player, file_track *file_track,
                                                        marker_pool *marker_pool, pasteboard *pasteboard,
                                                        database *database, editing_status const *editing_status) {
    return std::make_shared<track_editor>(player, file_track, marker_pool, pasteboard, database, editing_status);
}

track_editor::track_editor(player *player, file_track *file_track, marker_pool *marker_pool, pasteboard *pasteboard,
                           database *database, editing_status const *editing_status)
    : _player(player),
      _file_track(file_track),
      _marker_pool(marker_pool),
      _pasteboard(pasteboard),
      _database(database),
      _editing_status(editing_status) {
}

bool track_editor::can_split() const {
    if (!this->_editing_status->can_editing()) {
        return false;
    }

    auto const &file_track = this->_file_track;
    auto const current_frame = this->_player->current_frame();
    return file_track->splittable_module_at(current_frame).has_value();
}

void track_editor::split() {
    if (!this->can_split()) {
        return;
    }

    this->_database->suspend_saving([this] {
        auto const current_frame = this->_player->current_frame();
        this->_file_track->split_at(current_frame);
    });
}

void track_editor::drop_head() {
    if (!this->can_split()) {
        return;
    }

    this->_database->suspend_saving([this] {
        auto const current_frame = this->_player->current_frame();
        this->_file_track->drop_head_at(current_frame);
    });
}

void track_editor::drop_tail() {
    if (!this->can_split()) {
        return;
    }

    this->_database->suspend_saving([this] {
        auto const current_frame = this->_player->current_frame();
        this->_file_track->drop_tail_at(current_frame);
    });
}

void track_editor::drop_head_and_offset() {
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

void track_editor::drop_tail_and_offset() {
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

bool track_editor::can_erase() const {
    if (!this->_editing_status->can_editing()) {
        return false;
    }

    auto const current_frame = this->_player->current_frame();
    return this->_file_track->module_at(current_frame).has_value();
}

void track_editor::erase() {
    if (!this->can_erase()) {
        return;
    }

    this->_database->suspend_saving([this] {
        auto const current_frame = this->_player->current_frame();
        this->_file_track->erase_at(current_frame);
    });
}

void track_editor::erase_and_offset() {
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

bool track_editor::can_cut() const {
    return this->can_copy();
}

void track_editor::cut() {
    if (!this->can_cut()) {
        return;
    }

    this->_database->suspend_saving([this] {
        this->copy();

        auto const current_frame = this->_player->current_frame();
        this->_file_track->erase_at(current_frame);
    });
}

void track_editor::cut_and_offset() {
    if (!this->can_cut()) {
        return;
    }

    this->_database->suspend_saving([this] {
        this->copy();

        auto const current_frame = this->_player->current_frame();
        this->_file_track->erase_and_offset_at(current_frame);
    });
}

bool track_editor::can_copy() const {
    if (!this->_editing_status->can_editing()) {
        return false;
    }

    auto const &file_track = this->_file_track;
    auto const current_frame = this->_player->current_frame();
    return file_track->module_at(current_frame).has_value();
}

void track_editor::copy() {
    if (!this->can_copy()) {
        return;
    }

    this->_database->suspend_saving([this] {
        auto const &file_track = this->_file_track;
        auto const current_frame = this->_player->current_frame();
        if (auto const file_module = file_track->module_at(current_frame)) {
            auto const &value = file_module.value();
            this->_pasteboard->set_file_module({.name = value.name,
                                                .file_frame = value.file_frame,
                                                .length = value.range.length,
                                                .file_name = value.file_name});
        }
    });
}

bool track_editor::can_paste() const {
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

void track_editor::paste_and_offset() {
    if (!this->can_paste()) {
        return;
    }

    if (auto const module = this->_pasteboard->file_module()) {
        this->_database->suspend_saving([this, &module] {
            auto const module_value = module.value();
            auto const current_frame = this->_player->current_frame();

            this->_file_track->split_and_insert_module_and_offset({.name = module_value.name,
                                                                   .file_frame = module_value.file_frame,
                                                                   .range = {current_frame, module_value.length},
                                                                   .file_name = module_value.file_name});

            this->_marker_pool->move_offset_from(current_frame, module_value.length);
        });
    }
}

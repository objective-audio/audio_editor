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

#include <audio_editor_core/ae_selected_module_pool.hpp>

using namespace yas;
using namespace yas::ae;

track_editor::track_editor(player *player, file_track *file_track, marker_pool *marker_pool,
                           selected_module_pool *selected_pool, pasteboard *pasteboard, database *database,
                           editing_status const *editing_status)
    : _player(player),
      _file_track(file_track),
      _marker_pool(marker_pool),
      _selected_pool(selected_pool),
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

    this->_selected_pool->clear();

    auto const current_frame = this->_player->current_frame();
    this->_file_track->split_at(current_frame);
}

void track_editor::drop_head() {
    if (!this->can_split()) {
        return;
    }

    auto const current_frame = this->_player->current_frame();
    this->_file_track->drop_head_at(current_frame);
}

void track_editor::drop_tail() {
    if (!this->can_split()) {
        return;
    }

    this->_selected_pool->clear();

    auto const current_frame = this->_player->current_frame();
    this->_file_track->drop_tail_at(current_frame);
}

void track_editor::drop_head_and_offset() {
    if (!this->can_split()) {
        return;
    }

    this->_selected_pool->clear();

    auto const current_frame = this->_player->current_frame();
    auto const seek_frame = this->_file_track->module_at(current_frame).value().value.range.frame;

    auto const module_range = this->_file_track->module_at(current_frame)->value.range;
    this->_file_track->drop_head_and_offset_at(current_frame);
    auto const dropping_length = current_frame - module_range.frame;
    this->_marker_pool->erase({module_range.frame, static_cast<proc::length_t>(dropping_length)});
    auto const offset = -dropping_length;
    this->_marker_pool->move_offset_from(current_frame, offset);

    this->_player->seek(seek_frame);
}

void track_editor::drop_tail_and_offset() {
    if (!this->can_split()) {
        return;
    }

    this->_selected_pool->clear();

    auto const current_frame = this->_player->current_frame();
    auto const module_range = this->_file_track->module_at(current_frame)->value.range;
    this->_file_track->drop_tail_and_offset_at(current_frame);
    auto const dropping_length = module_range.next_frame() - current_frame;
    this->_marker_pool->erase({current_frame, static_cast<proc::length_t>(dropping_length)});
    auto const offset = -dropping_length;
    this->_marker_pool->move_offset_from(module_range.next_frame(), offset);
}

bool track_editor::can_erase() const {
    if (!this->_editing_status->can_editing()) {
        return false;
    }

    return this->_has_target_modules();
}

void track_editor::erase() {
    if (!this->can_erase()) {
        return;
    }

    auto selected_modules = this->_selected_pool->modules();
    this->_erase_modules(std::move(selected_modules));
}

bool track_editor::can_cut() const {
    return this->can_copy();
}

void track_editor::cut() {
    if (!this->can_cut()) {
        return;
    }

    // copyでクリアされるので先に保持しておく
    auto selected_modules = this->_selected_pool->modules();
    this->copy();
    this->_erase_modules(std::move(selected_modules));
}

bool track_editor::can_copy() const {
    if (!this->_editing_status->can_editing()) {
        return false;
    }

    return this->_has_target_modules();
}

void track_editor::copy() {
    if (!this->can_copy()) {
        return;
    }

    auto const &file_track = this->_file_track;
    auto const current_frame = this->_player->current_frame();
    auto const selected_modules = this->_selected_pool->modules();

    if (!selected_modules.empty()) {
        this->_selected_pool->clear();

        std::vector<pasting_module_object> pasting_modules;

        for (auto const &pair : selected_modules) {
            if (auto const module = file_track->module_at(pair.first)) {
                auto const &value = module.value().value;
                pasting_modules.emplace_back(pasting_module_object{
                    identifier{}, {value.name, value.file_frame, value.range.offset(-current_frame), value.file_name}});
            }
        }

        this->_pasteboard->set_modules(pasting_modules);
    } else {
        if (auto const module = file_track->module_at(current_frame)) {
            auto const &value = module.value().value;
            this->_pasteboard->set_modules(
                {{identifier{}, {value.name, value.file_frame, value.range.offset(-current_frame), value.file_name}}});
        }
    }
}

bool track_editor::can_paste() const {
    if (!this->_editing_status->can_editing()) {
        return false;
    }

    if (this->_pasteboard->modules().empty()) {
        return false;
    }

    return true;
}

void track_editor::paste() {
    if (!this->can_paste()) {
        return;
    }

    this->_selected_pool->clear();

    auto const &modules = this->_pasteboard->modules();
    if (modules.empty()) {
        return;
    }

    auto const current_frame = this->_player->current_frame();

    for (auto const &module : modules) {
        auto const &module_value = module.value;
        this->_file_track->overwrite_module({module_value.name, module_value.range.offset(current_frame),
                                             module_value.file_frame, module_value.file_name});
    }
}

bool track_editor::_has_target_modules() const {
    if (!this->_selected_pool->modules().empty()) {
        return true;
    } else {
        auto const current_frame = this->_player->current_frame();
        return this->_file_track->module_at(current_frame).has_value();
    }
}

void track_editor::_erase_modules(selected_module_map &&selected_modules) {
    if (!selected_modules.empty()) {
        this->_selected_pool->clear();

        for (auto const &pair : selected_modules) {
            this->_file_track->erase_module_and_notify(pair.first);
        }
    } else {
        auto const current_frame = this->_player->current_frame();
        this->_file_track->erase_at(current_frame);
    }
}

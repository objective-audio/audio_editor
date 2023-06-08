//
//  ae_module_editor.cpp
//

#include "ae_module_editor.h"

#include <audio_editor_core/ae_edge_holder.h>
#include <audio_editor_core/ae_editing_status.h>
#include <audio_editor_core/ae_file_info_loader.h>
#include <audio_editor_core/ae_hierarchy.h>
#include <audio_editor_core/ae_marker_pool.h>
#include <audio_editor_core/ae_module_pool.h>
#include <audio_editor_core/ae_pasteboard.h>
#include <audio_editor_core/ae_player.h>
#include <audio_editor_core/ae_time_editing_closer.h>
#include <audio_editor_core/ae_time_editing_lifetime.h>
#include <audio_editor_core/ae_time_editor.h>
#include <audio_editor_core/ae_timeline_holder_utils.h>
#include <cpp_utils/yas_fast_each.h>

#include <processing/yas_processing_umbrella.hpp>

using namespace yas;
using namespace yas::ae;

module_editor::module_editor(player *player, module_pool *module_pool, marker_pool *marker_pool,
                             selected_module_pool *selected_pool, pasteboard *pasteboard,
                             editing_status const *editing_status)
    : _player(player),
      _module_pool(module_pool),
      _marker_pool(marker_pool),
      _selected_pool(selected_pool),
      _pasteboard(pasteboard),
      _editing_status(editing_status) {
}

bool module_editor::can_split() const {
    if (!this->_editing_status->can_editing()) {
        return false;
    }

    auto const &module_pool = this->_module_pool;
    auto const current_frame = this->_player->current_frame();
    return module_pool->splittable_module_at(current_frame).has_value();
}

void module_editor::split() {
    if (!this->can_split()) {
        return;
    }

    this->_selected_pool->clear();

    auto const current_frame = this->_player->current_frame();
    this->_module_pool->split_at(current_frame);
}

void module_editor::drop_head() {
    if (!this->can_split()) {
        return;
    }

    auto const current_frame = this->_player->current_frame();
    this->_module_pool->drop_head_at(current_frame);
}

void module_editor::drop_tail() {
    if (!this->can_split()) {
        return;
    }

    this->_selected_pool->clear();

    auto const current_frame = this->_player->current_frame();
    this->_module_pool->drop_tail_at(current_frame);
}

void module_editor::drop_head_and_offset() {
    if (!this->can_split()) {
        return;
    }

    this->_selected_pool->clear();

    auto const current_frame = this->_player->current_frame();
    auto const seek_frame = this->_module_pool->module_at(current_frame).value().value.range.frame;

    auto const module_range = this->_module_pool->module_at(current_frame)->value.range;
    this->_module_pool->drop_head_and_offset_at(current_frame);
    auto const dropping_length = current_frame - module_range.frame;
    this->_marker_pool->erase({module_range.frame, static_cast<proc::length_t>(dropping_length)});
    auto const offset = -dropping_length;
    this->_marker_pool->move_offset_from(current_frame, offset);

    this->_player->seek(seek_frame);
}

void module_editor::drop_tail_and_offset() {
    if (!this->can_split()) {
        return;
    }

    this->_selected_pool->clear();

    auto const current_frame = this->_player->current_frame();
    auto const module_range = this->_module_pool->module_at(current_frame)->value.range;
    this->_module_pool->drop_tail_and_offset_at(current_frame);
    auto const dropping_length = module_range.next_frame() - current_frame;
    this->_marker_pool->erase({current_frame, static_cast<proc::length_t>(dropping_length)});
    auto const offset = -dropping_length;
    this->_marker_pool->move_offset_from(module_range.next_frame(), offset);
}

bool module_editor::can_erase() const {
    if (!this->_editing_status->can_editing()) {
        return false;
    }

    return this->_has_target_modules();
}

void module_editor::erase() {
    if (!this->can_erase()) {
        return;
    }

    auto selected_modules = this->_selected_pool->elements();
    this->_erase_modules(std::move(selected_modules));
}

bool module_editor::can_cut() const {
    return this->can_copy();
}

void module_editor::cut() {
    if (!this->can_cut()) {
        return;
    }

    // copyでクリアされるので先に保持しておく
    auto selected_modules = this->_selected_pool->elements();
    this->copy();
    this->_erase_modules(std::move(selected_modules));
}

bool module_editor::can_copy() const {
    if (!this->_editing_status->can_editing()) {
        return false;
    }

    return this->_has_target_modules();
}

void module_editor::copy() {
    if (!this->can_copy()) {
        return;
    }

    auto const &module_pool = this->_module_pool;
    auto const current_frame = this->_player->current_frame();
    auto const selected_modules = this->_selected_pool->elements();

    if (!selected_modules.empty()) {
        this->_selected_pool->clear();

        std::vector<pasting_module_object> pasting_modules;

        for (auto const &pair : selected_modules) {
            if (auto const module = module_pool->module_at(pair.first)) {
                auto const &value = module.value().value;
                pasting_modules.emplace_back(pasting_module_object{
                    identifier{}, {value.name, value.file_frame, value.range.offset(-current_frame), value.file_name}});
            }
        }

        this->_pasteboard->set_modules(pasting_modules);
    } else {
        if (auto const module = module_pool->module_at(current_frame)) {
            auto const &value = module.value().value;
            this->_pasteboard->set_modules(
                {{identifier{}, {value.name, value.file_frame, value.range.offset(-current_frame), value.file_name}}});
        }
    }
}

bool module_editor::can_paste() const {
    if (!this->_editing_status->can_editing()) {
        return false;
    }

    if (this->_pasteboard->modules().empty()) {
        return false;
    }

    return true;
}

void module_editor::paste() {
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
        this->_module_pool->overwrite_module({module_value.name, module_value.range.offset(current_frame),
                                              module_value.file_frame, module_value.file_name});
    }
}

bool module_editor::_has_target_modules() const {
    if (!this->_selected_pool->elements().empty()) {
        return true;
    } else {
        auto const current_frame = this->_player->current_frame();
        return this->_module_pool->module_at(current_frame).has_value();
    }
}

void module_editor::_erase_modules(selected_module_map &&selected_modules) {
    if (!selected_modules.empty()) {
        this->_selected_pool->clear();

        for (auto const &pair : selected_modules) {
            this->_module_pool->erase_module_and_notify(pair.first);
        }
    } else {
        auto const current_frame = this->_player->current_frame();
        this->_module_pool->erase_at(current_frame);
    }
}

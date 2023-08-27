//
//  ae_marker_editor.cpp
//

#include "ae_marker_editor.h"

#include <audio_editor_core/ae_database.h>
#include <audio_editor_core/ae_editing_status.h>
#include <audio_editor_core/ae_hierarchy.h>
#include <audio_editor_core/ae_marker_pool.h>
#include <audio_editor_core/ae_pasteboard.h>
#include <audio_editor_core/ae_player.h>

using namespace yas;
using namespace yas::ae;

marker_editor::marker_editor(player const *player, marker_pool *marker_pool, database *database,
                             editing_status const *editing_status, selected_marker_pool *selected_pool,
                             pasteboard *pasteboard)
    : _player(player),
      _marker_pool(marker_pool),
      _database(database),
      _editing_status(editing_status),
      _selected_pool(selected_pool),
      _pasteboard(pasteboard) {
}

bool marker_editor::can_insert() const {
    if (!this->_editing_status->can_editing()) {
        return false;
    }

    return true;
}

void marker_editor::insert() {
    if (!this->can_insert()) {
        return;
    }

    auto const current_frame = this->_player->current_frame();
    this->_marker_pool->insert(current_frame);
}

bool marker_editor::can_erase() const {
    if (!this->_editing_status->can_editing()) {
        return false;
    }

    return this->_has_target_markers();
}

void marker_editor::erase() {
    if (!this->can_erase()) {
        return;
    }

    auto selected_markers = this->_selected_pool->elements();

    this->_erase(std::move(selected_markers));
}

bool marker_editor::can_cut() const {
    return this->can_copy();
}

void marker_editor::cut() {
    if (!this->can_cut()) {
        return;
    }

    auto selected_markers = this->_selected_pool->elements();
    this->copy();
    this->_erase(std::move(selected_markers));
}

bool marker_editor::can_copy() const {
    if (!this->_editing_status->can_editing()) {
        return false;
    }

    return this->_has_target_markers();
}

void marker_editor::copy() {
    if (!this->can_copy()) {
        return;
    }

    auto const &marker_pool = this->_marker_pool;
    auto const current_frame = this->_player->current_frame();
    auto const selected_modules = this->_selected_pool->elements();

    if (!selected_modules.empty()) {
        this->_selected_pool->clear();

        std::vector<pasting_marker_object> pasting_markers;

        for (auto const &index : selected_modules) {
            if (auto const marker = marker_pool->marker_for_index(index)) {
                auto const &value = marker.value().value;
                pasting_markers.emplace_back(
                    pasting_marker_object{identifier{}, {value.frame - current_frame, value.name}});
            }
        }

        this->_pasteboard->set_markers(pasting_markers);
    } else {
        auto const pasting_markers = to_vector<pasting_marker_object>(
            marker_pool->markers_for_frame(current_frame), [current_frame](marker_object const &object) {
                return pasting_marker_object{identifier{}, {object.value.frame - current_frame, object.value.name}};
            });

        this->_pasteboard->set_markers(pasting_markers);
    }
}

bool marker_editor::can_paste() const {
    if (!this->_editing_status->can_editing()) {
        return false;
    }

    if (this->_pasteboard->markers().empty()) {
        return false;
    }

    return true;
}

void marker_editor::paste() {
    if (!this->can_paste()) {
        return;
    }

    this->_selected_pool->clear();

    auto const &markers = this->_pasteboard->markers();
    if (markers.empty()) {
        return;
    }

    auto const current_frame = this->_player->current_frame();

    for (auto const &marker : markers) {
        auto const &marker_value = marker.value;
        this->_marker_pool->insert(marker_value.frame + current_frame, marker_value.name);
    }
}

bool marker_editor::_has_target_markers() const {
    if (!this->_selected_pool->elements().empty()) {
        return true;
    } else {
        auto const current_frame = this->_player->current_frame();
        return this->_marker_pool->marker_exists_for_frame(current_frame);
    }
}

void marker_editor::_erase(selected_marker_set &&selected_markers) {
    if (!selected_markers.empty()) {
        this->_selected_pool->clear();

        for (auto const &index : selected_markers) {
            this->_marker_pool->erase(index);
        }
    } else {
        auto const current_frame = this->_player->current_frame();
        this->_marker_pool->erase(current_frame);
    }
}

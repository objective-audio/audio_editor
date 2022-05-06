//
//  ae_jumper.cpp
//

#include "ae_jumper.h"

#include <audio_editor_core/ae_edge_holder.h>
#include <audio_editor_core/ae_file_track.h>
#include <audio_editor_core/ae_hierarchy.h>
#include <audio_editor_core/ae_marker_pool.h>
#include <audio_editor_core/ae_player.h>
#include <cpp_utils/yas_assertion.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<jumper> jumper::make_shared(std::string const &project_id, file_track const *file_track,
                                            marker_pool const *marker_pool, edge_holder const *edge_holder) {
    auto const &project_level = hierarchy::project_level_for_id(project_id);
    return make_shared(project_level->player.get(), file_track, marker_pool, edge_holder);
}

std::shared_ptr<jumper> jumper::make_shared(player *player, file_track const *file_track,
                                            marker_pool const *marker_pool, edge_holder const *edge_holder) {
    return std::shared_ptr<jumper>(new jumper{player, file_track, marker_pool, edge_holder});
}

jumper::jumper(player *player, file_track const *file_track, marker_pool const *marker_pool,
               edge_holder const *edge_holder)
    : _player(player), _file_track(file_track), _marker_pool(marker_pool), _edge_holder(edge_holder) {
}

bool jumper::can_jump_to_previous_edge() const {
    return this->_previous_jumpable_frame().has_value();
}

bool jumper::can_jump_to_next_edge() const {
    return this->_next_jumpable_frame().has_value();
}

bool jumper::can_jump_to_beginnig() const {
    if (auto const edge = this->_first_edge().has_value()) {
        if (edge != this->_player->current_frame()) {
            return true;
        }
    }
    return false;
}

bool jumper::can_jump_to_end() const {
    if (auto const edge = this->_last_edge().has_value()) {
        if (edge != this->_player->current_frame()) {
            return true;
        }
    }
    return false;
}

void jumper::jump_to_previous_edge() {
    if (auto const edge = this->_previous_jumpable_frame()) {
        this->_player->seek(edge.value());
    }
}

void jumper::jump_to_next_edge() {
    if (auto const edge = this->_next_jumpable_frame()) {
        this->_player->seek(edge.value());
    }
}

void jumper::jump_to_beginning() {
    if (auto const edge = this->_first_edge()) {
        this->_player->seek(edge.value());
    }
}

void jumper::jump_to_end() {
    if (auto const edge = this->_last_edge()) {
        this->_player->seek(edge.value());
    }
}

std::optional<frame_index_t> jumper::_previous_jumpable_frame() const {
    frame_index_t const current_frame = this->_player->current_frame();

    std::optional<frame_index_t> result{std::nullopt};

    std::initializer_list<jumpable_on_project_editor const *> const editors{this->_file_track, this->_marker_pool,
                                                                            this->_edge_holder};

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

std::optional<frame_index_t> jumper::_next_jumpable_frame() const {
    frame_index_t const current_frame = this->_player->current_frame();

    std::optional<frame_index_t> result{std::nullopt};

    std::initializer_list<jumpable_on_project_editor const *> const editors{this->_file_track, this->_marker_pool,
                                                                            this->_edge_holder};

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

std::optional<frame_index_t> jumper::_first_edge() const {
    if (auto const module = this->_file_track->first_module()) {
        return module.value().range.frame;
    } else {
        return std::nullopt;
    }
}

std::optional<frame_index_t> jumper::_last_edge() const {
    if (auto const module = this->_file_track->last_module()) {
        return module.value().range.next_frame();
    } else {
        return std::nullopt;
    }
}
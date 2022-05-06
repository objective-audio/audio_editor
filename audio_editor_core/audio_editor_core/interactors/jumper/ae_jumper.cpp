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

std::shared_ptr<jumper> jumper::make_shared(std::string const &project_id,
                                            std::shared_ptr<file_track> const &file_track,
                                            std::shared_ptr<marker_pool> const &marker_pool,
                                            std::shared_ptr<edge_holder> const &edge_holder) {
    auto const &project_level = hierarchy::project_level_for_id(project_id);
    return make_shared(project_level->player, file_track, marker_pool, edge_holder);
}

std::shared_ptr<jumper> jumper::make_shared(std::shared_ptr<player> const &player,
                                            std::shared_ptr<file_track> const &file_track,
                                            std::shared_ptr<marker_pool> const &marker_pool,
                                            std::shared_ptr<edge_holder> const &edge_holder) {
    return std::shared_ptr<jumper>(new jumper{player, file_track, marker_pool, edge_holder});
}

jumper::jumper(std::shared_ptr<player> const &player, std::shared_ptr<file_track> const &file_track,
               std::shared_ptr<marker_pool> const &marker_pool, std::shared_ptr<edge_holder> const &edge_holder)
    : _player(player), _file_track(file_track), _marker_pool(marker_pool), _edge_holder(edge_holder) {
}

bool jumper::can_jump_to_previous_edge() const {
    return this->_previous_jumpable_frame().has_value();
}

bool jumper::can_jump_to_next_edge() const {
    return this->_next_jumpable_frame().has_value();
}

bool jumper::can_jump_to_beginnig() const {
    auto const player = this->_player.lock();

    if (!player) {
        assertion_failure_if_not_test();
        return false;
    }

    if (auto const edge = this->_first_edge().has_value()) {
        if (edge != player->current_frame()) {
            return true;
        }
    }
    return false;
}

bool jumper::can_jump_to_end() const {
    auto const player = this->_player.lock();

    if (!player) {
        assertion_failure_if_not_test();
        return false;
    }

    if (auto const edge = this->_last_edge().has_value()) {
        if (edge != player->current_frame()) {
            return true;
        }
    }
    return false;
}

void jumper::jump_to_previous_edge() {
    auto const player = this->_player.lock();

    if (!player) {
        assertion_failure_if_not_test();
        return;
    }

    if (auto const edge = this->_previous_jumpable_frame()) {
        player->seek(edge.value());
    }
}

void jumper::jump_to_next_edge() {
    auto const player = this->_player.lock();

    if (!player) {
        assertion_failure_if_not_test();
        return;
    }

    if (auto const edge = this->_next_jumpable_frame()) {
        player->seek(edge.value());
    }
}

void jumper::jump_to_beginning() {
    auto const player = this->_player.lock();

    if (!player) {
        assertion_failure_if_not_test();
        return;
    }

    if (auto const edge = this->_first_edge()) {
        player->seek(edge.value());
    }
}

void jumper::jump_to_end() {
    auto const player = this->_player.lock();

    if (!player) {
        assertion_failure_if_not_test();
        return;
    }

    if (auto const edge = this->_last_edge()) {
        player->seek(edge.value());
    }
}

std::optional<frame_index_t> jumper::_previous_jumpable_frame() const {
    auto const player = this->_player.lock();
    auto const file_track = this->_file_track.lock();
    auto const marker_pool = this->_marker_pool.lock();
    auto const edge_holder = this->_edge_holder.lock();

    if (!player || !file_track || !marker_pool || !edge_holder) {
        assertion_failure_if_not_test();
        return std::nullopt;
    }

    frame_index_t const current_frame = player->current_frame();

    std::optional<frame_index_t> result{std::nullopt};

    std::initializer_list<std::shared_ptr<jumpable_on_project_editor>> const editors{file_track, marker_pool,
                                                                                     edge_holder};

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
    auto const player = this->_player.lock();
    auto const file_track = this->_file_track.lock();
    auto const marker_pool = this->_marker_pool.lock();
    auto const edge_holder = this->_edge_holder.lock();

    if (!player || !file_track || !marker_pool || !edge_holder) {
        assertion_failure_if_not_test();
        return std::nullopt;
    }

    frame_index_t const current_frame = player->current_frame();

    std::optional<frame_index_t> result{std::nullopt};

    std::initializer_list<std::shared_ptr<jumpable_on_project_editor>> const editors{file_track, marker_pool,
                                                                                     edge_holder};

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
    auto const file_track = this->_file_track.lock();

    if (!file_track) {
        assertion_failure_if_not_test();
        return std::nullopt;
    }

    if (auto const module = file_track->first_module()) {
        return module.value().range.frame;
    } else {
        return std::nullopt;
    }
}

std::optional<frame_index_t> jumper::_last_edge() const {
    auto const file_track = this->_file_track.lock();

    if (!file_track) {
        assertion_failure_if_not_test();
        return std::nullopt;
    }

    if (auto const module = file_track->last_module()) {
        return module.value().range.next_frame();
    } else {
        return std::nullopt;
    }
}

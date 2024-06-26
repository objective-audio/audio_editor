//
//  jumper.cpp
//

#include "jumper.h"

#include <ae-core/global/hierarchy.h>
#include <ae-core/project/features/player.h>
#include <ae-core/project_editing/features/edge_holder.h>
#include <ae-core/project_editing/features/marker_pool.h>
#include <ae-core/project_editing/features/module_pool.h>
#include <cpp-utils/assertion.h>

using namespace yas;
using namespace yas::ae;

jumper::jumper(player *player, module_pool const *module_pool, marker_pool const *marker_pool,
               edge_holder const *edge_holder)
    : _player(player), _module_pool(module_pool), _marker_pool(marker_pool), _edge_holder(edge_holder) {
}

bool jumper::can_jump_to_previous_edge() const {
    return this->_previous_jumpable_frame().has_value();
}

bool jumper::can_jump_to_next_edge() const {
    return this->_next_jumpable_frame().has_value();
}

bool jumper::can_jump_to_beginning() const {
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

bool jumper::can_return_to_zero() const {
    return this->_player->current_frame() != 0;
}

void jumper::return_to_zero() {
    if (!this->can_return_to_zero()) {
        return;
    }

    this->_player->seek(0);
}

bool jumper::can_go_to_marker(std::size_t const marker_idx) const {
    auto const &marker_pool = this->_marker_pool;
    if (auto const marker = marker_pool->marker_at(marker_idx)) {
        return this->_player->current_frame() != marker->value.frame;
    } else {
        return false;
    }
}

void jumper::go_to_marker(std::size_t const marker_idx) {
    if (!this->can_go_to_marker(marker_idx)) {
        return;
    }

    auto const &marker_pool = this->_marker_pool;
    if (auto const marker = marker_pool->marker_at(marker_idx)) {
        this->_player->seek(marker->value.frame);
    }
}

#pragma mark - Private

std::optional<frame_index_t> jumper::_previous_jumpable_frame() const {
    frame_index_t const current_frame = this->_player->current_frame();

    std::optional<frame_index_t> result{std::nullopt};

    std::initializer_list<jumpable_on_jumper const *> const editors{this->_module_pool, this->_marker_pool,
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

    std::initializer_list<jumpable_on_jumper const *> const editors{this->_module_pool, this->_marker_pool,
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
    return this->_module_pool->first_frame();
}

std::optional<frame_index_t> jumper::_last_edge() const {
    return this->_module_pool->last_next_frame();
}

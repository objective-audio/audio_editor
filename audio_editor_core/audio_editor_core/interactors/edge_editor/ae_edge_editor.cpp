//
//  ae_edge_editor.cpp
//

#include "ae_edge_editor.h"

#include <audio_editor_core/ae_edge_holder.h>
#include <audio_editor_core/ae_exporter.h>
#include <audio_editor_core/ae_hierarchy.h>
#include <audio_editor_core/ae_player.h>
#include <cpp_utils/yas_assertion.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<edge_editor> edge_editor::make_shared(std::string const &project_id,
                                                      std::shared_ptr<edge_holder> const &holder,
                                                      std::shared_ptr<exporter> const &exporter) {
    auto const &project_level = hierarchy::project_level_for_id(project_id);
    return std::shared_ptr<edge_editor>(new edge_editor{holder, project_level->player, exporter});
}

edge_editor::edge_editor(std::shared_ptr<edge_holder> const &holder, std::shared_ptr<player> const &player,
                         std::shared_ptr<exporter> const &exporter)
    : _holder(holder), _player(player), _exporter(exporter) {
}

bool edge_editor::can_set_begin() const {
    auto const player = this->_player.lock();
    auto const holder = this->_holder.lock();

    if (!player || !holder) {
        assertion_failure_if_not_test();
        return false;
    }

    if (!this->_can_editing()) {
        return false;
    }

    auto const current_frame = player->current_frame();
    auto const begin_frame = holder->edge().begin_frame;

    return current_frame != begin_frame;
}

bool edge_editor::can_set_end() const {
    auto const player = this->_player.lock();
    auto const holder = this->_holder.lock();

    if (!player || !holder) {
        assertion_failure_if_not_test();
        return false;
    }

    if (!this->_can_editing()) {
        return false;
    }

    auto const current_frame = player->current_frame();
    auto const end_frame = holder->edge().end_frame;

    return current_frame != end_frame;
}

void edge_editor::set_begin() {
    if (!this->can_set_begin()) {
        return;
    }

    auto const player = this->_player.lock();
    auto const holder = this->_holder.lock();

    if (!player || !holder) {
        assertion_failure_if_not_test();
        return;
    }

    auto const current_frame = player->current_frame();
    holder->set_begin_frame(current_frame);
}

void edge_editor::set_end() {
    if (!this->can_set_end()) {
        return;
    }

    auto const player = this->_player.lock();
    auto const holder = this->_holder.lock();

    if (!player || !holder) {
        assertion_failure_if_not_test();
        return;
    }

    auto const current_frame = player->current_frame();
    holder->set_end_frame(current_frame);
}

bool edge_editor::_can_editing() const {
    auto const exporter = this->_exporter.lock();

    if (!exporter) {
        assertion_failure_if_not_test();
        return false;
    }

    return !exporter->is_exporting();
}

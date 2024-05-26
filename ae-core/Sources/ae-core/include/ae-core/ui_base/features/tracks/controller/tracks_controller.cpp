//
//  tracks_controller.cpp
//

#include "tracks_controller.hpp"

#include <ae-core/global/hierarchy.h>
#include <ae-core/project_editing/features/project_action_sender.h>
#include <cpp-utils/assertion.h>

#include <ae-core/project_editing/features/deselector.hpp>
#include <ae-core/project_editing/features/range_selector.hpp>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<tracks_controller> tracks_controller::make_shared(project_lifetime_id const &project_lifetime_id) {
    auto const &project_editing_lifetime = hierarchy::project_editing_lifetime_for_id(project_lifetime_id);
    return std::make_shared<tracks_controller>(
        project_editing_lifetime->action_sender, project_editing_lifetime->track_content_pool,
        project_editing_lifetime->range_selector, project_editing_lifetime->deselector);
}

tracks_controller::tracks_controller(std::shared_ptr<project_action_sender> const &action_sender,
                                     std::shared_ptr<track_content_pool> const &content_pool,
                                     std::shared_ptr<range_selector> const &range_selector,
                                     std::shared_ptr<deselector> const &deselector)
    : _action_sender(action_sender),
      _content_pool(content_pool),
      _range_selector(range_selector),
      _deselector(deselector) {
}

void tracks_controller::deselect_all() {
    if (auto const deselector = this->_deselector.lock()) {
        deselector->deselect_all();
    }
}

void tracks_controller::begin_range_selection(ui::point const &position) {
    if (auto const range_selector = this->_range_selector.lock()) {
        range_selector->begin(position);
    }
}

void tracks_controller::begin_selection() {
    if (auto const action_sender = this->_action_sender.lock()) {
        action_sender->send(editing_action_name::begin_tracks_selection);
    }
}

void tracks_controller::select(std::vector<std::size_t> const &content_indices) {
    if (auto const action_sender = this->_action_sender.lock()) {
        auto track_indices = this->_track_indices(content_indices);
        action_sender->send(editing_action_name::select_tracks, std::move(track_indices));
    }
}

void tracks_controller::end_selection() {
    if (auto const action_sender = this->_action_sender.lock()) {
        action_sender->send(editing_action_name::end_tracks_selection);
    }
}

void tracks_controller::toggle_selection(std::size_t const content_idx) {
    if (auto const action_sender = this->_action_sender.lock()) {
        if (auto track_idx = this->_track_index(content_idx)) {
            action_sender->send(editing_action_name::toggle_track_selection, std::move(track_idx.value()));
        }
    }
}

std::optional<track_index_t> tracks_controller::_track_index(std::size_t const content_idx) {
    auto const track_indices = this->_track_indices({content_idx});
    if (track_indices.empty()) {
        return std::nullopt;
    } else {
        return *track_indices.begin();
    }
}

std::set<track_index_t> tracks_controller::_track_indices(std::vector<std::size_t> const &content_indices) {
    auto const content_pool = this->_content_pool.lock();
    if (!content_pool) {
        assertion_failure_if_not_test();
        return {};
    }

    std::set<track_index_t> track_indices;

    auto const &contents = content_pool->elements();

    for (auto const &idx : content_indices) {
        if (idx < contents.size()) {
            auto const &content = contents.at(idx);
            if (content.has_value()) {
                track_indices.emplace(content.value().track());
            }
        }
    }

    return track_indices;
}

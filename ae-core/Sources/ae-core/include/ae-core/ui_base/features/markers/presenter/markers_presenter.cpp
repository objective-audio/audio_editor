//
//  markers_presenter.cpp
//

#include "markers_presenter.h"

#include <ae-core/global/hierarchy.h>
#include <ae-core/project/features/display_space.h>
#include <ae-core/project/features/player.h>
#include <ae-core/project_editing/features/marker_pool.h>
#include <ae-core/project_editing/features/module_editor.h>
#include <cpp-utils/lock.h>

#include <ae-core/project_editing/features/display_space_time_range/display_space_time_range.hpp>
#include <ae-core/project_editing/features/range_selector.hpp>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<markers_presenter> markers_presenter::make_shared(
    project_lifetime_id const &project_lifetime_id, std::shared_ptr<marker_content_pool> const &content_pool) {
    auto const &project_lifetime = hierarchy::project_lifetime_for_id(project_lifetime_id);
    auto const &project_editing_lifetime = hierarchy::project_editing_lifetime_for_id(project_lifetime_id);
    return std::make_shared<markers_presenter>(
        project_editing_lifetime->project_format, project_lifetime->player, project_editing_lifetime->marker_pool,
        project_editing_lifetime->selected_marker_pool, project_lifetime->display_space,
        project_editing_lifetime->display_space_time_range, content_pool, project_editing_lifetime->range_selector);
}

markers_presenter::markers_presenter(project_format const &project_format, std::shared_ptr<player> const &player,
                                     std::shared_ptr<marker_pool> const &marker_pool,
                                     std::shared_ptr<selected_marker_pool> const &selected_marker_pool,
                                     std::shared_ptr<display_space> const &display_space,
                                     std::shared_ptr<display_space_time_range> const &display_space_time_range,
                                     std::shared_ptr<marker_content_pool> const &content_pool,
                                     std::shared_ptr<range_selector> const &range_selector)
    : _project_format(project_format),
      _player(player),
      _marker_pool(marker_pool),
      _selected_marker_pool(selected_marker_pool),
      _display_space(display_space),
      _display_space_time_range(display_space_time_range),
      _content_pool(content_pool),
      _range_selector(range_selector) {
    auto const sample_rate = this->_project_format.sample_rate;

    marker_pool
        ->observe_event([this, sample_rate](marker_pool_event const &event) {
            auto const display_space = this->_display_space.lock();

            if (!display_space) {
                return;
            }

            switch (event.type) {
                case marker_pool_event_type::any:
                case marker_pool_event_type::reverted:
                    this->_update_all_contents(update_type::update);
                    break;
                case marker_pool_event_type::erased:
                    this->_content_pool->erase_for_id(event.erased.value().identifier);
                    break;
                case marker_pool_event_type::inserted: {
                    auto const &inserted = event.inserted.value();
                    this->_insert_content_if_in_space_range(inserted.index());
                } break;
                case marker_pool_event_type::replaced: {
                    auto const &inserted = event.inserted.value();
                    auto const &erased = event.erased.value();

                    this->_replace_or_erase_content_if_in_space_range(inserted.index(), erased.index());
                } break;
            }
        })
        .sync()
        ->add_to(this->_canceller_pool);

    selected_marker_pool
        ->observe_event([this](selected_marker_pool::event const &event) {
            switch (event.type) {
                case selected_pool_event_type::fetched:
                    // marker_poolの通知に任せてendにしているので呼ばれない
                    break;
                case selected_pool_event_type::replaced:
                    this->_update_all_contents(update_type::update);
                    break;
                case selected_pool_event_type::toggled:
                    this->_replace_contents_if_in_space_range(event.toggled);
                    break;
            }
        })
        .end()  // marker_poolのフェッチで反映されているのでend
        ->add_to(this->_canceller_pool);

    display_space
        ->observe([this](display_space_event const &event) {
            switch (event.source) {
                case display_space_event_source::fetched:
                    // marker_poolの通知に任せてendにしているので呼ばれない
                    break;
                case display_space_event_source::view:
                case display_space_event_source::scale:
                    this->_update_all_contents(update_type::replace);
                    break;
            }
        })
        .end()
        ->add_to(this->_canceller_pool);
}

std::vector<std::optional<marker_content>> const &markers_presenter::contents() const {
    return this->_content_pool->elements();
}

observing::syncable markers_presenter::observe_contents(
    std::function<void(marker_content_pool_event const &)> &&handler) {
    return this->_content_pool->observe_event(std::move(handler));
}

observing::syncable markers_presenter::observe_range(std::function<void(range_selection const &)> &&handler) {
    if (auto const range_selector = this->_range_selector.lock()) {
        return range_selector->observe(range_selection_order::markers, std::move(handler));
    } else {
        return observing::syncable{};
    }
}

std::optional<marker_index> markers_presenter::marker_index_at(std::size_t const content_idx) const {
    if (content_idx < this->contents().size()) {
        if (auto const &content = this->contents().at(content_idx); content.has_value()) {
            if (auto const marker_pool = this->_marker_pool.lock()) {
                if (auto const marker = marker_pool->marker_for_id(content.value().identifier)) {
                    return marker.value().index();
                }
            }
        }
    }

    return std::nullopt;
}

void markers_presenter::update_if_needed() {
    this->_update_all_contents(update_type::update_if_changed);
}

std::optional<time::range> markers_presenter::_space_range() const {
    if (auto const space_range = this->_display_space_time_range.lock()) {
        return space_range->current();
    } else {
        return std::nullopt;
    }
}

void markers_presenter::_update_all_contents(update_type const type) {
    auto const space_range = this->_space_range();
    auto const locked = yas::lock(this->_marker_pool, this->_player, this->_display_space);

    if (!space_range.has_value() || !fulfilled(locked)) {
        return;
    }

    auto const &[marker_pool, player, display_space] = locked;
    auto const current_frame = player->current_frame();

    if (space_range == this->_last_space_range && current_frame == this->_last_frame &&
        type == update_type::update_if_changed) {
        return;
    }

    auto const scale = display_space->scale();
    auto const &space_range_value = space_range.value();

    auto const contents = filter_map<marker_content>(
        marker_pool->markers(),
        [this, &space_range_value, sample_rate = this->_project_format.sample_rate, &scale](auto const &pair) {
            auto const &marker = pair.second;
            bool const is_selected = this->_is_selected({marker.identifier, marker.value.frame});
            if (space_range_value.is_contain(pair.second.value.frame)) {
                return std::make_optional<marker_content>(pair.second.identifier, pair.second.value.frame, sample_rate,
                                                          scale.width, pair.second.value.name, is_selected);
            } else {
                return std::optional<marker_content>(std::nullopt);
            }
        });

    switch (type) {
        case update_type::replace:
            this->_content_pool->replace_all(contents);
            break;
        case update_type::update:
            this->_content_pool->update_all(contents, true);
            break;
        case update_type::update_if_changed:
            this->_content_pool->update_all(contents, false);
            break;
    }

    this->_last_frame = current_frame;
    this->_last_space_range = space_range;
}

void markers_presenter::_insert_content_if_in_space_range(marker_index const &index) {
    auto const space_range = this->_space_range();
    auto const locked = yas::lock(this->_marker_pool, this->_display_space);

    if (!space_range.has_value() || !fulfilled(locked)) {
        return;
    }

    auto const &[marker_pool, display_space] = locked;

    auto const marker = marker_pool->marker_for_index(index);
    if (!marker.has_value()) {
        return;
    }

    auto const &marker_value = marker.value();

    if (space_range.value().is_contain(marker_value.value.frame)) {
        bool const is_selected = this->_is_selected({marker_value.identifier, marker_value.value.frame});
        this->_content_pool->insert({marker_value.identifier, marker_value.value.frame,
                                     this->_project_format.sample_rate, display_space->scale().width,
                                     marker_value.value.name, is_selected});
    }
}

void markers_presenter::_replace_or_erase_content_if_in_space_range(marker_index const &inserted_index,
                                                                    marker_index const &erased_index) {
    auto const space_range = this->_space_range();
    auto const locked = yas::lock(this->_marker_pool, this->_display_space);

    if (!space_range.has_value() || !fulfilled(locked)) {
        return;
    }

    auto const &[marker_pool, display_space] = locked;

    auto const inserted = marker_pool->marker_for_index(inserted_index);
    if (!inserted.has_value()) {
        return;
    }

    auto const &inserted_value = inserted.value();

    if (space_range.value().is_contain(inserted_index.frame)) {
        bool const is_selected = this->_is_selected({inserted_value.identifier, inserted_value.value.frame});
        this->_content_pool->insert_or_replace({inserted_value.identifier, inserted_value.value.frame,
                                                this->_project_format.sample_rate, display_space->scale().width,
                                                inserted_value.value.name, is_selected});
    } else {
        this->_content_pool->erase_for_id(erased_index.object_id);
    }
}

void markers_presenter::_replace_contents_if_in_space_range(selected_marker_set const &changed) {
    auto const space_range = this->_space_range();
    auto const locked = yas::lock(this->_marker_pool, this->_display_space);

    if (!space_range.has_value() || !fulfilled(locked)) {
        return;
    }

    auto const &[marker_pool, display_space] = locked;

    for (auto const &index : changed) {
        auto const marker = marker_pool->marker_for_index(index);
        if (!marker.has_value()) {
            continue;
        }

        auto const &marker_value = marker.value();

        if (space_range.value().is_contain(marker_value.value.frame)) {
            bool const is_selected = this->_is_selected({marker_value.identifier, marker_value.value.frame});
            this->_content_pool->replace({marker_value.identifier, marker_value.value.frame,
                                          this->_project_format.sample_rate, display_space->scale().width,
                                          marker_value.value.name, is_selected});
        }
    }
}

bool markers_presenter::_is_selected(marker_index const &index) const {
    if (auto const selected_pool = this->_selected_marker_pool.lock()) {
        return selected_pool->elements().contains(index);
    }
    return false;
}

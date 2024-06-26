//
//  tracks_presenter.cpp
//

#include "tracks_presenter.hpp"

#include <ae-core/global/hierarchy.h>
#include <ae-core/project/features/display_space.h>
#include <cpp-utils/fast_each.h>
#include <cpp-utils/lock.h>

#include <ae-core/project_editing/features/display_space_range/display_space_range.hpp>
#include <ae-core/project_editing/features/range_selector.hpp>

using namespace yas;
using namespace yas::ae;

static std::vector<std::optional<track_content>> const _empty_contents;

std::shared_ptr<tracks_presenter> tracks_presenter::make_shared(project_lifetime_id const &project_lifetime_id) {
    auto const &project_lifetime = hierarchy::project_lifetime_for_id(project_lifetime_id);
    auto const &project_editing_lifetime = hierarchy::project_editing_lifetime_for_id(project_lifetime_id);
    return std::make_shared<tracks_presenter>(
        project_lifetime_id, project_editing_lifetime->track_content_pool,
        project_editing_lifetime->selected_track_pool, project_lifetime->display_space,
        project_editing_lifetime->display_space_range, project_editing_lifetime->range_selector);
}

tracks_presenter::tracks_presenter(project_lifetime_id const &project_lifetime_id,
                                   std::shared_ptr<track_content_pool> const &content_pool,
                                   std::shared_ptr<selected_track_pool> const &selected_pool,
                                   std::shared_ptr<display_space> const &display_space,
                                   std::shared_ptr<display_space_range> const &display_space_range,
                                   std::shared_ptr<range_selector> const &range_selector)
    : _content_pool(content_pool),
      _selected_pool(selected_pool),
      _display_space(display_space),
      _display_space_range(display_space_range),
      _range_selector(range_selector) {
    selected_pool
        ->observe_event([this](selected_track_pool::event const &event) {
            switch (event.type) {
                case selected_pool_event_type::fetched:
                case selected_pool_event_type::replaced:
                    this->_update_all_contents(update_type::update);
                    break;
                case selected_pool_event_type::toggled:
                    this->_replace_contents(event.toggled);
                    break;
            }
        })
        .sync()
        ->add_to(this->_canceller_pool);

    display_space
        ->observe([this](display_space_event const &event) {
            switch (event.source) {
                case display_space_event_source::fetched:
                    // selected_poolの通知に任せてendにしているので呼ばれない
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

std::vector<std::optional<track_content>> const &tracks_presenter::contents() const {
    if (auto const content_pool = this->_content_pool.lock()) {
        return content_pool->elements();
    } else {
        return _empty_contents;
    }
}

observing::syncable tracks_presenter::observe_contents(
    std::function<void(track_content_pool_event const &)> &&handler) {
    if (auto const content_pool = this->_content_pool.lock()) {
        return content_pool->observe_event(std::move(handler));
    } else {
        return observing::syncable{};
    }
}

observing::syncable tracks_presenter::observe_range(std::function<void(range_selection const &)> &&handler) {
    if (auto const range_selector = this->_range_selector.lock()) {
        return range_selector->observe(range_selection_order::tracks, std::move(handler));
    } else {
        return observing::syncable{};
    }
}

void tracks_presenter::update_if_needed() {
    // 毎フレーム呼ばれて、位置が変わっていたら更新する
    this->_update_all_contents(update_type::update_if_changed);
}

std::optional<ae::space_range> tracks_presenter::_space_range() const {
    if (auto const display_space_range = this->_display_space_range.lock()) {
        auto const space_range = display_space_range->current();
        if (space_range.has_value()) {
            return space_range.value();
        }
    }
    return std::nullopt;
}

void tracks_presenter::_replace_contents(selected_track_set const &changed) {
    auto const locked = yas::lock(this->_content_pool, this->_selected_pool, this->_display_space);

    if (!fulfilled(locked)) {
        return;
    }

    auto const &[content_pool, selected_pool, display_space] = locked;
    auto const space_range = this->_space_range();

    if (space_range.has_value()) {
        auto const &space_track_range = space_range.value().track_range;
        auto const height_scale = display_space->scale().height;

        for (auto const &track_idx : changed) {
            if (space_track_range.contains(track_idx)) {
                content_pool->replace(track_content{
                    .identifier = track_idx, .is_selected = selected_pool->contains(track_idx), .scale = height_scale});
            }
        }
    }
}

void tracks_presenter::_update_all_contents(update_type const type) {
    auto const space_range = this->_space_range();
    auto const locked = yas::lock(this->_content_pool, this->_selected_pool, this->_display_space);

    if (space_range.has_value() && fulfilled(locked)) {
        auto const &[content_pool, selected_pool, display_space] = locked;

        if (space_range == this->_last_space_range && type == update_type::update_if_changed) {
            return;
        }

        std::vector<track_content> contents;
        float const height_scale = display_space->scale().height;

        auto const &track_range = space_range->track_range;
        auto each = make_fast_each(track_range.min, track_range.max + 1);
        while (yas_each_next(each)) {
            auto const &track_idx = yas_each_index(each);
            contents.emplace_back(track_content{
                .identifier = track_idx, .is_selected = selected_pool->contains(track_idx), .scale = height_scale});
        }

        switch (type) {
            case update_type::replace:
                content_pool->replace_all(contents);
                break;
            case update_type::update:
                content_pool->update_all(contents, true);
                break;
            case update_type::update_if_changed:
                content_pool->update_all(contents, false);
                break;
        }

        this->_last_space_range = space_range;
    }
}

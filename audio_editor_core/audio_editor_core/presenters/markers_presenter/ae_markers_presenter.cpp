//
//  ae_markers_presenter.cpp
//

#include "ae_markers_presenter.h"

#include <audio_editor_core/ae_display_space.h>
#include <audio_editor_core/ae_hierarchy.h>
#include <audio_editor_core/ae_marker_pool.h>
#include <audio_editor_core/ae_player.h>
#include <audio_editor_core/ae_track_editor.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<markers_presenter> markers_presenter::make_shared(
    window_lifetime_id const &window_lifetime_id, std::shared_ptr<display_space> const &display_space,
    std::shared_ptr<marker_content_pool> const &content_pool) {
    auto const &window_lifetime = hierarchy::window_lifetime_for_id(window_lifetime_id);
    auto const &project_lifetime = hierarchy::project_lifetime_for_id(window_lifetime_id);
    return std::make_shared<markers_presenter>(project_lifetime->project_format, window_lifetime->player,
                                               project_lifetime->marker_pool, display_space, content_pool);
}

markers_presenter::markers_presenter(project_format const &project_format, std::shared_ptr<player> const &player,
                                     std::shared_ptr<marker_pool> const &marker_pool,
                                     std::shared_ptr<display_space> const &display_space,
                                     std::shared_ptr<marker_content_pool> const &content_pool)
    : _project_format(project_format),
      _player(player),
      _marker_pool(marker_pool),
      _display_space(display_space),
      _content_pool(content_pool) {
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
                    auto const &marker = event.inserted.value();
                    if (auto const space_range = this->_space_range();
                        space_range.has_value() && space_range.value().is_contain(marker.value.frame)) {
                        this->_content_pool->insert(
                            {marker.identifier, marker.value.frame, sample_rate, display_space->scale().width});
                    }
                } break;
                case marker_pool_event_type::replaced: {
                    auto const &inserted = event.inserted.value();
                    auto const &erased = event.erased.value();

                    if (inserted.value.frame != erased.value.frame) {
                        if (auto const space_range = this->_space_range();
                            space_range.has_value() && space_range.value().is_contain(inserted.value.frame)) {
                            this->_content_pool->insert_or_replace(
                                {inserted.identifier, inserted.value.frame, sample_rate, display_space->scale().width});
                        } else {
                            this->_content_pool->erase_for_id(erased.identifier);
                        }
                    }
                } break;
            }
        })
        .sync()
        ->add_to(this->_canceller_pool);

    display_space
        ->observe([this](display_space_event const &event) {
            switch (event.source) {
                case display_space_event_source::fetched:
                    this->_update_all_contents(update_type::update);
                    break;
                case display_space_event_source::view:
                case display_space_event_source::scale:
                    this->_update_all_contents(update_type::replace);
                    break;
            }
        })
        .sync()
        ->add_to(this->_canceller_pool);
}

std::vector<std::optional<marker_content>> markers_presenter::contents() const {
    return this->_content_pool->elements();
}

observing::syncable markers_presenter::observe_contents(
    std::function<void(marker_content_pool_event const &)> &&handler) {
    return this->_content_pool->observe_event(std::move(handler));
}

void markers_presenter::update_if_needed() {
    this->_update_all_contents(update_type::update_if_changed);
}

std::optional<time::range> markers_presenter::_space_range() const {
    auto const player = this->_player.lock();
    auto const display_space = this->_display_space.lock();

    if (player && display_space) {
        auto const sample_rate = this->_project_format.sample_rate;
        auto const current_frame = player->current_frame();
        return display_space->frame_range(sample_rate, current_frame);
    } else {
        return std::nullopt;
    }
}

void markers_presenter::_update_all_contents(update_type const type) {
    auto const space_range = this->_space_range();
    auto const marker_pool = this->_marker_pool.lock();
    auto const player = this->_player.lock();
    auto const display_space = this->_display_space.lock();

    if (marker_pool && player && space_range.has_value() && display_space) {
        auto const scale = display_space->scale();
        auto const current_frame = player->current_frame();

        if (space_range == this->_last_space_range && current_frame == this->_last_frame &&
            type == update_type::update_if_changed) {
            return;
        }

        auto const &space_range_value = space_range.value();

        auto const contents = filter_map<marker_content>(
            marker_pool->markers(),
            [&space_range_value, sample_rate = this->_project_format.sample_rate, &scale](auto const &pair) {
                if (space_range_value.is_contain(pair.second.value.frame)) {
                    return std::make_optional<marker_content>(pair.second.identifier, pair.second.value.frame,
                                                              sample_rate, scale.width);
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
}

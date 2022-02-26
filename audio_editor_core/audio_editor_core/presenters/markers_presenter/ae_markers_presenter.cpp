//
//  ae_markers_presenter.cpp
//

#include "ae_markers_presenter.h"

#include <audio_editor_core/ae_app_level.h>
#include <audio_editor_core/ae_display_space.h>
#include <audio_editor_core/ae_project.h>
#include <audio_editor_core/ae_project_editor.h>
#include <audio_editor_core/ae_project_editor_level.h>
#include <audio_editor_core/ae_project_editor_level_pool.h>
#include <audio_editor_core/ae_project_level.h>
#include <audio_editor_core/ae_project_level_pool.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<markers_presenter> markers_presenter::make_shared(std::string const &project_id,
                                                                  std::shared_ptr<display_space> const &display_space) {
    auto const &project_level = app_level::global()->project_pool->project_level_for_id(project_id);
    auto const &editor_level = project_level->editor_level_pool->editor_level();
    auto const &file_info = editor_level->file_info;
    auto const &editor = editor_level->editor;
    return make_shared(file_info, editor, display_space);
}

std::shared_ptr<markers_presenter> markers_presenter::make_shared(
    file_info const &file_info, std::shared_ptr<project_editor_for_markers_presenter> const &editor,
    std::shared_ptr<display_space> const &display_space) {
    return std::shared_ptr<markers_presenter>(new markers_presenter{file_info, editor, display_space});
}

markers_presenter::markers_presenter(file_info const &file_info,
                                     std::shared_ptr<project_editor_for_markers_presenter> const &editor,
                                     std::shared_ptr<display_space> const &display_space)
    : _file_info(file_info),
      _project_editor(editor),
      _display_space(display_space),
      _location_pool(marker_location_pool::make_shared()) {
    auto const sample_rate = this->_file_info.sample_rate;

    editor
        ->observe_marker_pool_event([this, sample_rate](marker_pool_event const &event) {
            switch (event.type) {
                case marker_pool_event_type::any:
                case marker_pool_event_type::reverted:
                    this->_update_all_locations(update_type::update);
                    break;
                case marker_pool_event_type::erased:
                    this->_location_pool->erase(event.marker.value().identifier);
                    break;
                case marker_pool_event_type::inserted:
                    auto const &marker = event.marker.value();
                    if (auto const space_range = this->_space_range();
                        space_range.has_value() && space_range.value().is_contain(marker.frame)) {
                        this->_location_pool->insert(marker_location::make_value(
                            marker.identifier, marker.frame, sample_rate, this->_display_space->scale()));
                    }
                    break;
            }
        })
        .sync()
        ->add_to(this->_canceller_pool);

    display_space
        ->observe([this](display_space_event const &event) {
            switch (event.source) {
                case display_space_event_source::fetched:
                case display_space_event_source::view:
                    this->_update_all_locations(update_type::update);
                    break;
                case display_space_event_source::scale:
                    this->_update_all_locations(update_type::replace);
                    break;
            }
        })
        .sync()
        ->add_to(this->_canceller_pool);
}

std::vector<std::optional<marker_location>> markers_presenter::locations() const {
    return this->_location_pool->elements();
}

observing::syncable markers_presenter::observe_locations(
    std::function<void(marker_location_pool_event const &)> &&handler) {
    return this->_location_pool->observe_event(std::move(handler));
}

void markers_presenter::update_if_needed() {
    this->_update_all_locations(update_type::update_if_changed);
}

std::optional<time::range> markers_presenter::_space_range() const {
    if (auto const editor = this->_project_editor.lock()) {
        auto const sample_rate = this->_file_info.sample_rate;
        auto const current_frame = editor->current_frame();
        return this->_display_space->frame_range(sample_rate, current_frame);
    } else {
        return std::nullopt;
    }
}

void markers_presenter::_update_all_locations(update_type const type) {
    auto const space_range = this->_space_range();
    auto const editor = this->_project_editor.lock();

    if (editor && space_range.has_value()) {
        auto const current_frame = editor->current_frame();

        if (space_range == this->_last_space_range && current_frame == this->_last_frame &&
            type == update_type::update_if_changed) {
            return;
        }

        auto const &space_range_value = space_range.value();

        auto const locations = filter_map<marker_location>(
            editor->markers(), [&space_range_value, sample_rate = this->_file_info.sample_rate,
                                scale = this->_display_space->scale()](auto const &pair) {
                if (space_range_value.is_contain(pair.second.frame)) {
                    return std::make_optional(
                        marker_location::make_value(pair.second.identifier, pair.second.frame, sample_rate, scale));
                } else {
                    return std::optional<marker_location>(std::nullopt);
                }
            });

        switch (type) {
            case update_type::replace:
                this->_location_pool->replace_all(locations);
                break;
            case update_type::update:
            case update_type::update_if_changed:
                this->_location_pool->update_all(locations);
                break;
        }

        this->_last_frame = current_frame;
        this->_last_space_range = space_range;
    }
}

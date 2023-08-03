//
//  ae_timeline_holder.cpp
//

#include "ae_timeline_holder.h"

#include <audio_editor_core/ae_hierarchy.h>
#include <audio_editor_core/ae_project_path.h>
#include <audio_editor_core/ae_timeline_holder_utils.h>

#include <processing/yas_processing_umbrella.hpp>

using namespace yas;
using namespace yas::ae;

timeline_holder::timeline_holder(project_format const &project_format, project_path const *project_path)
    : _project_format(project_format), _project_path(project_path), _timeline(proc::timeline::make_shared()) {
}

proc::timeline_ptr const &timeline_holder::timeline() const {
    return this->_timeline;
}

void timeline_holder::replace(module_pool_module_map_t const &modules) {
    this->_timeline->erase_all_tracks();

    proc::timeline_track_map_t tracks;

    for (auto const &pair : modules) {
        auto const &module = pair.second.value;
        auto const &track_idx = module.track;
        auto const ch_count = this->_project_format.channel_count;

        if (!tracks.contains(track_idx)) {
            tracks.emplace(track_idx, proc::track::make_shared());
        }

        auto const &track = tracks.at(track_idx);
        track->push_back_module(timeline_holder_utils::make_module(module, this->_project_path, ch_count),
                                module.range);
    }

    this->_timeline->replace_tracks(std::move(tracks));
}

void timeline_holder::insert(module const &module) {
    auto const &track_idx = module.track;

    if (!this->_timeline->has_track(track_idx)) {
        this->_timeline->insert_track(track_idx, proc::track::make_shared());
    }

    auto const &track = this->_timeline->track(track_idx);
    auto const ch_count = this->_project_format.channel_count;

    track->push_back_module(timeline_holder_utils::make_module(module, this->_project_path, ch_count), module.range);
}

void timeline_holder::erase(track_index_t const track_idx, time::range const &range) {
    if (this->_timeline->has_track(track_idx)) {
        auto const &track = this->_timeline->track(track_idx);

        track->erase_modules_for_range(range);

        if (track->module_sets().empty()) {
            this->_timeline->erase_track(track_idx);
        }
    }
}

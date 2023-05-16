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
    : _project_format(project_format),
      _project_path(project_path),
      _timeline(proc::timeline::make_shared()),
      _track(proc::track::make_shared()) {
    this->_timeline->insert_track(0, this->_track);
}

proc::timeline_ptr const &timeline_holder::timeline() const {
    return this->_timeline;
}

void timeline_holder::replace(module_pool_module_map_t const &modules) {
    this->_timeline->erase_track(0);
    this->_track = proc::track::make_shared();

    auto const ch_count = this->_project_format.channel_count;

    for (auto const &pair : modules) {
        auto const &module = pair.second.value;
        this->_track->push_back_module(timeline_holder_utils::make_module(module, this->_project_path, ch_count),
                                       module.range);
    }

    this->_timeline->insert_track(0, this->_track);
}

void timeline_holder::insert(module const &module) {
    if (auto const &track = this->_track) {
        auto const ch_count = this->_project_format.channel_count;

        track->push_back_module(timeline_holder_utils::make_module(module, this->_project_path, ch_count),
                                module.range);
    } else {
        assert(0);
    }
}

void timeline_holder::erase(time::range const &range) {
    if (auto const &track = this->_track) {
        track->erase_modules_for_range(range);
    }
}

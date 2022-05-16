//
//  ae_timeline_holder.cpp
//

#include "ae_timeline_holder.h"

#include <audio_editor_core/ae_hierarchy.h>
#include <audio_editor_core/ae_project_url.h>
#include <audio_editor_core/ae_timeline_holder_utils.h>
#include <processing/yas_processing_umbrella.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<timeline_holder> timeline_holder::make_shared(url const &editing_file_url,
                                                              project_format const &project_format) {
    return std::make_shared<timeline_holder>(editing_file_url, project_format);
}

timeline_holder::timeline_holder(url const &editing_file_url, project_format const &project_format)
    : _timeline(proc::timeline::make_shared()),
      _track(proc::track::make_shared()),
      _editing_file_url(editing_file_url),
      _project_format(project_format) {
    this->_timeline->insert_track(0, this->_track);
}

proc::timeline_ptr const &timeline_holder::timeline() const {
    return this->_timeline;
}

void timeline_holder::replace(file_track_module_map_t const &modules) {
    this->_timeline->erase_track(0);
    this->_track = proc::track::make_shared();

    auto const url = this->_editing_file_url;
    auto const ch_count = this->_project_format.channel_count;

    for (auto const &pair : modules) {
        auto const &file_module = pair.second;
        this->_track->push_back_module(timeline_holder_utils::make_module(file_module, url, ch_count),
                                       file_module.range);
    }

    this->_timeline->insert_track(0, this->_track);
}

void timeline_holder::insert(file_module const &file_module) {
    if (auto const &track = this->_track) {
        auto const url = this->_editing_file_url;
        auto const ch_count = this->_project_format.channel_count;

        track->push_back_module(timeline_holder_utils::make_module(file_module, url, ch_count), file_module.range);
    } else {
        assert(0);
    }
}

void timeline_holder::erase(time::range const &range) {
    if (auto const &track = this->_track) {
        track->erase_modules_for_range(range);
    }
}

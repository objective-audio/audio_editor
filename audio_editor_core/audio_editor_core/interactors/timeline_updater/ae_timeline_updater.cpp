//
//  ae_timeline_updater.cpp
//

#include "ae_timeline_updater.h"

#include <audio_editor_core/ae_hierarchy.h>
#include <audio_editor_core/ae_project_editor_utils.h>
#include <audio_editor_core/ae_project_url.h>
#include <processing/yas_processing_umbrella.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<timeline_updater> timeline_updater::make_shared(project_id const &project_id,
                                                                ae::file_info const &file_info) {
    auto const &project_level = hierarchy::project_level_for_id(project_id);
    auto const &project_url = project_level->project_url;
    return make_shared(project_url->editing_file(), file_info);
}

std::shared_ptr<timeline_updater> timeline_updater::make_shared(url const &editing_file_url,
                                                                ae::file_info const &file_info) {
    return std::shared_ptr<timeline_updater>(new timeline_updater{editing_file_url, file_info});
}

timeline_updater::timeline_updater(url const &editing_file_url, ae::file_info const &file_info)
    : _timeline(proc::timeline::make_shared()),
      _track(proc::track::make_shared()),
      _editing_file_url(editing_file_url),
      _file_info(file_info) {
    this->_timeline->insert_track(0, this->_track);
}

proc::timeline_ptr const &timeline_updater::timeline() const {
    return this->_timeline;
}

void timeline_updater::replace(file_track_module_map_t const &modules) {
    this->_timeline->erase_track(0);
    this->_track = proc::track::make_shared();

    auto const url = this->_editing_file_url;
    auto const ch_count = this->_file_info.channel_count;

    for (auto const &pair : modules) {
        auto const &file_module = pair.second;
        this->_track->push_back_module(project_editor_utils::make_module(file_module, url, ch_count),
                                       file_module.range);
    }

    this->_timeline->insert_track(0, this->_track);
}

void timeline_updater::insert(file_module const &file_module) {
    if (auto const &track = this->_track) {
        auto const url = this->_editing_file_url;
        auto const ch_count = this->_file_info.channel_count;

        track->push_back_module(project_editor_utils::make_module(file_module, url, ch_count), file_module.range);
    } else {
        assert(0);
    }
}

void timeline_updater::erase(time::range const &range) {
    if (auto const &track = this->_track) {
        track->erase_modules_for_range(range);
    }
}

//
//  ae_project_editor.cpp
//

#include "ae_project_editor.h"

#include <audio_editor_core/ae_app.h>
#include <audio_editor_core/ae_file_loader.h>
#include <audio_editor_core/ae_file_track.h>
#include <audio_editor_core/ae_marker_pool.h>
#include <audio_editor_core/ae_project_editor_utils.h>
#include <cpp_utils/yas_fast_each.h>
#include <processing/yas_processing_umbrella.h>

using namespace yas;
using namespace yas::ae;

project_editor::project_editor(url const &url, file_info const &file_info,
                               std::shared_ptr<project_editor_player_interface> const &player,
                               std::shared_ptr<project_editor_file_track_interface> const &file_track,
                               std::shared_ptr<project_editor_marker_pool_interface> const &marker_pool)
    : _url(url),
      _file_info(file_info),
      _player(player),
      _file_track(file_track),
      _timeline(proc::timeline::make_shared()),
      _track(proc::track::make_shared()),
      _marker_pool(marker_pool) {
    this->_timeline->insert_track(0, this->_track);
    this->_player->set_timeline(this->_timeline, file_info.sample_rate, audio::pcm_format::float32);

    this->_file_track
        ->observe_event([this](file_track_event const &event) {
            auto const &url = this->_url;
            auto const &ch_count = this->_file_info.channel_count;

            switch (event.type) {
                case file_track_event_type::any:
                    this->_timeline->erase_track(0);
                    this->_track = proc::track::make_shared();

                    for (auto const &pair : event.modules) {
                        auto const &file_module = pair.second;
                        this->_track->push_back_module(project_editor_utils::make_module(file_module, url, ch_count),
                                                       file_module.range);
                    }

                    this->_timeline->insert_track(0, this->_track);
                    break;
                case file_track_event_type::inserted:
                    if (auto const &track = this->_track) {
                        auto const &file_module = event.module.value();
                        track->push_back_module(project_editor_utils::make_module(file_module, url, ch_count),
                                                file_module.range);
                    }
                    break;
                case file_track_event_type::erased:
                    if (auto const &track = this->_track) {
                        auto const &file_module = event.module.value();
                        track->erase_modules_for_range(file_module.range);
                    }
                    break;
            }
        })
        .sync()
        ->add_to(this->_pool);

    this->_file_track->insert_module_and_notify(file_module{proc::time::range{0, file_info.length}, 0});
}

std::shared_ptr<project_editor_file_track_interface> const &project_editor::file_track() const {
    return this->_file_track;
}

std::shared_ptr<project_editor> project_editor::make_shared(
    url const &url, file_info const &file_info, std::shared_ptr<project_editor_player_interface> const &player) {
    return make_shared(url, file_info, player, file_track::make_shared(), marker_pool::make_shared());
}

std::shared_ptr<project_editor> project_editor::make_shared(
    url const &url, file_info const &file_info, std::shared_ptr<project_editor_player_interface> const &player,
    std::shared_ptr<project_editor_file_track_interface> const &file_track,
    std::shared_ptr<project_editor_marker_pool_interface> const &marker_pool) {
    return std::shared_ptr<project_editor>(new project_editor{url, file_info, player, file_track, marker_pool});
}

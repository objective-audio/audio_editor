//
//  ae_project_editor.cpp
//

#include "ae_project_editor.h"

#include <audio_editor_core/ae_app.h>
#include <audio_editor_core/ae_database.h>
#include <audio_editor_core/ae_file_loader.h>
#include <audio_editor_core/ae_file_track.h>
#include <audio_editor_core/ae_marker_pool.h>
#include <audio_editor_core/ae_project_editor_utils.h>
#include <cpp_utils/yas_fast_each.h>
#include <processing/yas_processing_umbrella.h>

using namespace yas;
using namespace yas::ae;

project_editor::project_editor(url const &editing_file_url, file_info const &file_info,
                               std::shared_ptr<player_for_project_editor> const &player,
                               std::shared_ptr<file_track_for_project_editor> const &file_track,
                               std::shared_ptr<marker_pool_for_project_editor> const &marker_pool,
                               std::shared_ptr<database_for_project_editor> const &database)
    : _editing_file_url(editing_file_url),
      _file_info(file_info),
      _player(player),
      _file_track(file_track),
      _timeline(proc::timeline::make_shared()),
      _track(proc::track::make_shared()),
      _marker_pool(marker_pool),
      _database(database) {
    this->_timeline->insert_track(0, this->_track);
    this->_player->set_timeline(this->_timeline, file_info.sample_rate, audio::pcm_format::float32);

    this->_file_track
        ->observe_event([this](file_track_event const &event) {
            auto const &url = this->_editing_file_url;
            auto const &ch_count = this->_file_info.channel_count;

            switch (event.type) {
                case file_track_event_type::any:
                    this->_timeline->erase_track(0);
                    this->_track = proc::track::make_shared();

                    for (auto const &pair : event.modules) {
                        auto const &file_module = pair.second;
                        this->_track->push_back_module(project_editor_utils::make_module(file_module, url, ch_count),
                                                       file_module.range);
                        this->_database->add_module(file_module);
                    }

                    this->_timeline->insert_track(0, this->_track);
                    this->_database->save();
                    break;
                case file_track_event_type::reverted:
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
                        this->_database->add_module(file_module);
                        this->_database->save();
                    }
                    break;
                case file_track_event_type::erased:
                    if (auto const &track = this->_track) {
                        auto const &file_module = event.module.value();
                        track->erase_modules_for_range(file_module.range);
                        this->_database->remove_module(file_module.range);
                        this->_database->save();
                    }
                    break;
            }
        })
        .sync()
        ->add_to(this->_pool);

    this->_database
        ->observe_reverted([this](db_modules_map const &db_modules) {
            std::vector<file_module> file_modules;

            for (auto const &pair : db_modules) {
                if (auto file_module = pair.second.file_module()) {
                    file_modules.emplace_back(std::move(file_module.value()));
                }
            }

            this->_file_track->revert_modules_and_notify(std::move(file_modules));
        })
        .end()
        ->add_to(this->_pool);

    this->_file_track->insert_module_and_notify(file_module{proc::time::range{0, file_info.length}, 0});
}

std::shared_ptr<file_track_for_project_editor> const &project_editor::file_track() const {
    return this->_file_track;
}

std::shared_ptr<marker_pool_for_project_editor> const &project_editor::marker_pool() const {
    return this->_marker_pool;
}

std::shared_ptr<database_for_project_editor> const &project_editor::database() const {
    return this->_database;
}

std::shared_ptr<project_editor> project_editor::make_shared(url const &editing_file_url, url const &db_file_url,
                                                            file_info const &file_info,
                                                            std::shared_ptr<player_for_project_editor> const &player) {
    return make_shared(editing_file_url, file_info, player, file_track::make_shared(), marker_pool::make_shared(),
                       database::make_shared(db_file_url));
}

std::shared_ptr<project_editor> project_editor::make_shared(
    url const &editing_file_url, file_info const &file_info, std::shared_ptr<player_for_project_editor> const &player,
    std::shared_ptr<file_track_for_project_editor> const &file_track,
    std::shared_ptr<marker_pool_for_project_editor> const &marker_pool,
    std::shared_ptr<database_for_project_editor> const &database) {
    return std::shared_ptr<project_editor>(
        new project_editor{editing_file_url, file_info, player, file_track, marker_pool, database});
}

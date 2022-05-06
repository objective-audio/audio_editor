//
//  ae_project_editor_launcher.cpp
//

#include "ae_project_editor_launcher.h"

#include <audio_editor_core/ae_database.h>
#include <audio_editor_core/ae_edge_holder.h>
#include <audio_editor_core/ae_file_track.h>
#include <audio_editor_core/ae_hierarchy.h>
#include <audio_editor_core/ae_player.h>
#include <audio_editor_core/ae_project_url.h>
#include <audio_editor_core/ae_timeline_updater.h>
#include <cpp_utils/yas_assertion.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<project_editor_launcher> project_editor_launcher::make_shared(
    std::string const &project_id, ae::file_info const &file_info, std::shared_ptr<timeline_updater> const &updater,
    std::shared_ptr<database> const &database, std::shared_ptr<file_track> const &file_track,
    std::shared_ptr<edge_holder> const &edge_holder) {
    auto const &project_level = hierarchy::project_level_for_id(project_id);
    auto const &project_url = project_level->project_url;
    return std::shared_ptr<project_editor_launcher>(new project_editor_launcher{
        file_info, project_url->editing_file(), project_level->player, updater, database, file_track, edge_holder});
}

project_editor_launcher::project_editor_launcher(ae::file_info const &file_info, url const &editing_file_url,
                                                 std::shared_ptr<player> const &player,
                                                 std::shared_ptr<timeline_updater> const &updater,
                                                 std::shared_ptr<database> const &database,
                                                 std::shared_ptr<file_track> const &file_track,
                                                 std::shared_ptr<edge_holder> const &edge_holder)
    : _file_info(file_info),
      _editing_file_url(editing_file_url),
      _player(player),
      _timeline_updater(updater),
      _database(database),
      _file_track(file_track),
      _edge_holder(edge_holder) {
}

void project_editor_launcher::launch() {
    auto const &file_info = this->_file_info;
    auto const &editing_file_url = this->_editing_file_url;
    auto const player = this->_player.lock();
    auto const processor = this->_timeline_updater.lock();
    auto const database = this->_database.lock();
    auto const file_track = this->_file_track.lock();
    auto const edge_holder = this->_edge_holder.lock();

    if (!player || !processor || !database || !file_track || !edge_holder) {
        assertion_failure_if_not_test();
        return;
    }

    // プロジェクトの初期状態を作る。事前にdbに直接挿入してrevertから始めるべきかもしれない。
    database->suspend_saving([&file_info, &file_track, &edge_holder, &editing_file_url] {
        file_track->insert_module_and_notify(file_module{.name = editing_file_url.last_path_component(),
                                                         .range = time::range{0, file_info.length},
                                                         .file_frame = 0});

        edge_holder->set_edge({.begin_frame = 0, .end_frame = static_cast<frame_index_t>(file_info.length)});
    });

    player->set_timeline(processor->timeline(), file_info.sample_rate, audio::pcm_format::float32);
    player->begin_rendering();

    this->_finalize();
}

void project_editor_launcher::_finalize() {
    this->_player.reset();
    this->_timeline_updater.reset();
    this->_database.reset();
    this->_file_track.reset();
    this->_edge_holder.reset();
}

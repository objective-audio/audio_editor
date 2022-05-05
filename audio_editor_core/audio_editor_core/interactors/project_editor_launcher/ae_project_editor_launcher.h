//
//  ae_project_editor_launcher.h
//

#pragma once

#include <audio_editor_core/ae_file_info.h>
#include <cpp_utils/yas_url.h>

namespace yas::ae {
class player;
class timeline_processor;
class database;
class file_track;
class edge_editor;

struct project_editor_launcher final {
    [[nodiscard]] static std::shared_ptr<project_editor_launcher> make_shared(
        std::string const &project_id, ae::file_info const &, std::shared_ptr<timeline_processor> const &,
        std::shared_ptr<database> const &, std::shared_ptr<file_track> const &, std::shared_ptr<edge_editor> const &);

    void launch();

   private:
    ae::file_info const _file_info;
    url const _editing_file_url;
    std::weak_ptr<player> _player;
    std::weak_ptr<timeline_processor> _timeline_processor;
    std::weak_ptr<database> _database;
    std::weak_ptr<file_track> _file_track;
    std::weak_ptr<edge_editor> _edge_editor;

    project_editor_launcher(ae::file_info const &, url const &editing_file_url, std::shared_ptr<player> const &,
                            std::shared_ptr<timeline_processor> const &, std::shared_ptr<database> const &,
                            std::shared_ptr<file_track> const &, std::shared_ptr<edge_editor> const &);

    void _finalize();
};
}  // namespace yas::ae

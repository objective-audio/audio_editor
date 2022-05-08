//
//  ae_project_editor_launcher.h
//

#pragma once

#include <audio_editor_core/ae_file_info.h>
#include <audio_editor_core/ae_project_id.h>
#include <cpp_utils/yas_url.h>

namespace yas::ae {
class player;
class timeline_holder;
class database;
class file_track;
class edge_holder;

struct project_editor_launcher final {
    [[nodiscard]] static std::shared_ptr<project_editor_launcher> make_shared(project_id const &project_id,
                                                                              ae::file_info const &, timeline_holder *,
                                                                              database *, file_track *, edge_holder *);

    project_editor_launcher(ae::file_info const &, url const &editing_file_url, player *, timeline_holder *, database *,
                            file_track *, edge_holder *);

    void launch();

   private:
    ae::file_info const _file_info;
    url const _editing_file_url;
    player *const _player;
    timeline_holder const *const _timeline_holder;
    database *const _database;
    file_track *const _file_track;
    edge_holder *const _edge_holder;
};
}  // namespace yas::ae

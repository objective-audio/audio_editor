//
//  ae_project_editor.h
//

#pragma once

#include <audio_editor_core/ae_project_dependency.h>
#include <audio_editor_core/ae_project_editor_dependency.h>

namespace yas::ae {
struct project_editor final : project_editor_for_project {
    [[nodiscard]] std::shared_ptr<file_track_for_project_editor> const &file_track() const override;

    [[nodiscard]] static std::shared_ptr<project_editor> make_shared(
        url const &, file_info const &, std::shared_ptr<player_for_project_editor> const &);
    [[nodiscard]] static std::shared_ptr<project_editor> make_shared(
        url const &, file_info const &, std::shared_ptr<player_for_project_editor> const &,
        std::shared_ptr<file_track_for_project_editor> const &,
        std::shared_ptr<marker_pool_for_project_editor> const &);

   private:
    url const _url;
    file_info const _file_info;
    std::shared_ptr<player_for_project_editor> const _player;
    std::shared_ptr<file_track_for_project_editor> const _file_track;
    std::shared_ptr<marker_pool_for_project_editor> const _marker_pool;

    proc::timeline_ptr const _timeline;
    proc::track_ptr _track;
    observing::canceller_pool _pool;

    project_editor(url const &, file_info const &, std::shared_ptr<player_for_project_editor> const &,
                   std::shared_ptr<file_track_for_project_editor> const &,
                   std::shared_ptr<marker_pool_for_project_editor> const &);

    project_editor(project_editor const &) = delete;
    project_editor(project_editor &&) = delete;
    project_editor &operator=(project_editor const &) = delete;
    project_editor &operator=(project_editor &&) = delete;
};
}  // namespace yas::ae

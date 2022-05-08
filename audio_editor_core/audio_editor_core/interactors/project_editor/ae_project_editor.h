//
//  ae_project_editor.h
//

#pragma once

#include <audio_editor_core/ae_project_editor_dependency.h>
#include <audio_editor_core/ae_project_id.h>
#include <audio_editor_core/ae_responder.h>

namespace yas::ae {
class timeline_updater;
class editing_status;

struct project_editor final {
    [[nodiscard]] static std::shared_ptr<project_editor> make_shared(
        project_id const &project_id, ae::file_info const &, file_track_for_project_editor *,
        marker_pool_for_project_editor *, edge_holder_for_project_editor *, pasteboard_for_project_editor *,
        database_for_project_editor *, timeline_updater *, editing_status const *);

    [[nodiscard]] bool can_split() const;
    void split();
    void drop_head();
    void drop_tail();
    void drop_head_and_offset();
    void drop_tail_and_offset();

    [[nodiscard]] bool can_erase() const;
    void erase();
    void erase_and_offset();

    [[nodiscard]] bool can_undo() const;
    void undo();

    [[nodiscard]] bool can_redo() const;
    void redo();

    [[nodiscard]] bool can_cut() const;
    void cut_and_offset();
    [[nodiscard]] bool can_copy() const;
    void copy();
    [[nodiscard]] bool can_paste() const;
    void paste_and_offset();

   private:
    identifier const _responder_id;
    ae::file_info const _file_info;
    player_for_project_editor *const _player;
    file_track_for_project_editor *const _file_track;
    marker_pool_for_project_editor *const _marker_pool;
    edge_holder_for_project_editor *const _edge_holder;
    pasteboard_for_project_editor *const _pasteboard;
    database_for_project_editor *const _database;
    timeline_updater *const _timeline_updater;
    editing_status const *const _editing_status;

    observing::canceller_pool _pool;

    project_editor(ae::file_info const &, player_for_project_editor *, file_track_for_project_editor *,
                   marker_pool_for_project_editor *, edge_holder_for_project_editor *, pasteboard_for_project_editor *,
                   database_for_project_editor *, timeline_updater *, editing_status const *);

    project_editor(project_editor const &) = delete;
    project_editor(project_editor &&) = delete;
    project_editor &operator=(project_editor const &) = delete;
    project_editor &operator=(project_editor &&) = delete;

    [[nodiscard]] std::optional<frame_index_t> _previous_jumpable_frame() const;
    [[nodiscard]] std::optional<frame_index_t> _next_jumpable_frame() const;
    [[nodiscard]] std::optional<frame_index_t> _first_edge() const;
    [[nodiscard]] std::optional<frame_index_t> _last_edge() const;
};
}  // namespace yas::ae

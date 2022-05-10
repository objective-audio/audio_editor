//
//  ae_project_editor.h
//

#pragma once

#include <audio_editor_core/ae_project_id.h>
#include <observing/yas_observing_umbrella.h>

namespace yas::ae {
class player;
class file_track;
class marker_pool;
class pasteboard;
class database;
class editing_status;

struct project_editor final {
    [[nodiscard]] static std::shared_ptr<project_editor> make_shared(project_id const &project_id, file_track *,
                                                                     marker_pool *, pasteboard *, database *,
                                                                     editing_status const *);

    project_editor(player *, file_track *, marker_pool *, pasteboard *, database *, editing_status const *);

    [[nodiscard]] bool can_split() const;
    void split();
    void drop_head();
    void drop_tail();
    void drop_head_and_offset();
    void drop_tail_and_offset();

    [[nodiscard]] bool can_erase() const;
    void erase();
    void erase_and_offset();

    [[nodiscard]] bool can_cut() const;
    void cut_and_offset();
    [[nodiscard]] bool can_copy() const;
    void copy();
    [[nodiscard]] bool can_paste() const;
    void paste_and_offset();

   private:
    player *const _player;
    file_track *const _file_track;
    marker_pool *const _marker_pool;
    pasteboard *const _pasteboard;
    database *const _database;
    editing_status const *const _editing_status;

    observing::canceller_pool _pool;

    project_editor(project_editor const &) = delete;
    project_editor(project_editor &&) = delete;
    project_editor &operator=(project_editor const &) = delete;
    project_editor &operator=(project_editor &&) = delete;
};
}  // namespace yas::ae

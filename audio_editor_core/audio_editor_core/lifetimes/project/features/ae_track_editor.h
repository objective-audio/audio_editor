//
//  ae_track_editor.h
//

#pragma once

#include <observing/yas_observing_umbrella.h>

namespace yas::ae {
class player;
class file_track;
class marker_pool;
class pasteboard;
class database;
class editing_status;

struct track_editor final {
    [[nodiscard]] static std::shared_ptr<track_editor> make_shared(player *, file_track *, marker_pool *, pasteboard *,
                                                                   database *, editing_status const *);

    track_editor(player *, file_track *, marker_pool *, pasteboard *, database *, editing_status const *);

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
    void cut();
    void cut_and_offset();
    [[nodiscard]] bool can_copy() const;
    void copy();
    [[nodiscard]] bool can_paste() const;
    void paste();
    void paste_and_offset();

   private:
    player *const _player;
    file_track *const _file_track;
    marker_pool *const _marker_pool;
    pasteboard *const _pasteboard;
    database *const _database;
    editing_status const *const _editing_status;

    observing::canceller_pool _pool;

    track_editor(track_editor const &) = delete;
    track_editor(track_editor &&) = delete;
    track_editor &operator=(track_editor const &) = delete;
    track_editor &operator=(track_editor &&) = delete;
};
}  // namespace yas::ae

//
//  ae_marker_editor.h
//

#pragma once

#include <audio_editor_core/ae_selected_marker_pool_types.h>

#include <memory>

namespace yas::ae {
class player;
class marker_pool;
class database;
class editing_status;
class selected_marker_pool;
class pasteboard;

struct marker_editor final {
    marker_editor(player const *, marker_pool *, database *, editing_status const *, selected_marker_pool *,
                  pasteboard *);

    [[nodiscard]] bool can_insert() const;
    void insert();

    [[nodiscard]] bool can_erase() const;
    void erase();

    [[nodiscard]] bool can_cut() const;
    void cut();
    [[nodiscard]] bool can_copy() const;
    void copy();
    [[nodiscard]] bool can_paste() const;
    void paste();

   private:
    player const *const _player;
    marker_pool *const _marker_pool;
    database *const _database;
    editing_status const *const _editing_status;
    selected_marker_pool *const _selected_pool;
    pasteboard *const _pasteboard;

    marker_editor(marker_editor const &) = delete;
    marker_editor(marker_editor &&) = delete;
    marker_editor &operator=(marker_editor const &) = delete;
    marker_editor &operator=(marker_editor &&) = delete;

    bool _has_target_markers() const;
    void _erase(selected_marker_map &&);
};
}  // namespace yas::ae

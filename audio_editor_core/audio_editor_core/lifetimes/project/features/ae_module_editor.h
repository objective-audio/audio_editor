//
//  ae_module_editor.h
//

#pragma once

#include <observing/yas_observing_umbrella.h>

#include <audio_editor_core/ae_selected_module_pool.hpp>

namespace yas::ae {
class player;
class module_pool;
class marker_pool;
class pasteboard;
class database;
class editing_status;

struct module_editor final {
    module_editor(player *, module_pool *, marker_pool *, selected_module_pool *, pasteboard *, database *,
                  editing_status const *);

    [[nodiscard]] bool can_split() const;
    void split();
    void drop_head();
    void drop_tail();
    void drop_head_and_offset();
    void drop_tail_and_offset();

    [[nodiscard]] bool can_erase() const;
    void erase();

    [[nodiscard]] bool can_cut() const;
    void cut();
    [[nodiscard]] bool can_copy() const;
    void copy();
    [[nodiscard]] bool can_paste() const;
    void paste();

   private:
    player *const _player;
    module_pool *const _module_pool;
    marker_pool *const _marker_pool;
    selected_module_pool *const _selected_pool;
    pasteboard *const _pasteboard;
    database *const _database;
    editing_status const *const _editing_status;

    observing::canceller_pool _pool;

    module_editor(module_editor const &) = delete;
    module_editor(module_editor &&) = delete;
    module_editor &operator=(module_editor const &) = delete;
    module_editor &operator=(module_editor &&) = delete;

    bool _has_target_modules() const;
    void _erase_modules(selected_module_map &&);
};
}  // namespace yas::ae

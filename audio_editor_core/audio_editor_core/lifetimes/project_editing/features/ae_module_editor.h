//
//  ae_module_editor.h
//

#pragma once

#include <audio_editor_core/ae_selected_module_pool.hpp>
#include <audio_editor_core/ae_selected_track_pool.hpp>
#include <audio_editor_core/ae_vertical_scrolling.hpp>
#include <observing/yas_observing_umbrella.hpp>

namespace yas::ae {
class player;
class module_pool;
class marker_pool;
class track_selector;
class pasteboard;
class database;
class editing_status;
class selector_enabler;
class vertical_scrolling;
enum class selector_kind;

struct module_editor final {
    module_editor(player *, module_pool *, marker_pool *, selected_module_pool *, selected_track_pool *,
                  vertical_scrolling const *, track_selector const *, pasteboard *, editing_status const *,
                  selector_enabler const *);

    [[nodiscard]] bool can_split() const;
    void split();
    void drop_head();
    void drop_tail();

    [[nodiscard]] bool can_erase() const;
    void erase();

    [[nodiscard]] bool can_cut() const;
    void cut();
    [[nodiscard]] bool can_copy() const;
    void copy();
    [[nodiscard]] bool can_paste() const;
    void paste();

   private:
    enum class target_kind {
        modules,
        markers,
        tracks,
    };

    player *const _player;
    module_pool *const _module_pool;
    marker_pool *const _marker_pool;
    selected_module_pool *const _selected_module_pool;
    selected_track_pool *_selected_track_pool;
    vertical_scrolling const *_vertical_scrolling;
    track_selector const *const _track_selector;
    pasteboard *const _pasteboard;
    editing_status const *const _editing_status;
    selector_enabler const *const _selector_enabler;

    observing::canceller_pool _pool;

    module_editor(module_editor const &) = delete;
    module_editor(module_editor &&) = delete;
    module_editor &operator=(module_editor const &) = delete;
    module_editor &operator=(module_editor &&) = delete;

    [[nodiscard]] target_kind _target_kind() const;
    [[nodiscard]] bool _has_target_modules() const;
    [[nodiscard]] bool _has_splittable_modules() const;
    void _erase_modules(selected_module_set &&);
};
}  // namespace yas::ae

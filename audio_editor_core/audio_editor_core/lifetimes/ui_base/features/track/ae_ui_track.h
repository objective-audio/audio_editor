//
//  ae_ui_track.h
//

#pragma once

#include <audio_editor_core/ae_window_lifetime_id.h>
#include <ui/yas_ui_umbrella.h>

namespace yas::ae {
class track_presenter;
class ui_modules;
class ui_pasting_modules;
class ui_markers;
class ui_edge;
class display_space;

struct ui_track final {
    [[nodiscard]] static std::shared_ptr<ui_track> make_shared(window_lifetime_id const &, ui_modules *,
                                                               ui_pasting_modules *);

    ui_track(std::shared_ptr<ui::standard> const &, display_space *, std::shared_ptr<track_presenter> const &,
             ui_modules *, ui_pasting_modules *);

   private:
    std::shared_ptr<track_presenter> const _presenter;

    display_space *const _display_space;
    ui_modules *const _modules;
    ui_pasting_modules *const _pasting_modules;

    observing::canceller_pool _pool;

    ui_track(ui_track const &) = delete;
    ui_track(ui_track &&) = delete;
    ui_track &operator=(ui_track const &) = delete;
    ui_track &operator=(ui_track &&) = delete;

    void _update_scale();
};
}  // namespace yas::ae

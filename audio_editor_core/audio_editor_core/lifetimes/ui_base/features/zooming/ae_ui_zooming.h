//
//  ae_ui_zooming.h
//

#pragma once

#include <audio_editor_core/ae_project_lifetime_id.h>
#include <ui/yas_ui_umbrella.h>

namespace yas::ae {
class zooming_presenter;
class ui_modules;
class ui_pasting_modules;
class ui_grid;
class ui_tracks;
class ui_tracks_bg;
class display_space;

struct ui_zooming final {
    [[nodiscard]] static std::shared_ptr<ui_zooming> make_shared(project_lifetime_id const &, ui_modules *,
                                                                 ui_pasting_modules *, ui_grid *, ui_tracks *,
                                                                 ui_tracks_bg *);

    ui_zooming(std::shared_ptr<ui::standard> const &, display_space *, std::shared_ptr<zooming_presenter> const &,
               ui_modules *, ui_pasting_modules *, ui_grid *, ui_tracks *, ui_tracks_bg *);

   private:
    std::shared_ptr<zooming_presenter> const _presenter;

    display_space *const _display_space;
    ui_modules *const _modules;
    ui_pasting_modules *const _pasting_modules;
    ui_grid *const _grid;
    ui_tracks *const _tracks;
    ui_tracks_bg *const _tracks_bg;

    observing::canceller_pool _pool;

    ui_zooming(ui_zooming const &) = delete;
    ui_zooming(ui_zooming &&) = delete;
    ui_zooming &operator=(ui_zooming const &) = delete;
    ui_zooming &operator=(ui_zooming &&) = delete;

    void _update_scale();
};
}  // namespace yas::ae

//
//  ae_ui_track.h
//

#pragma once

#include <audio_editor_core/ae_window_lifetime_id.h>
#include <ui/yas_ui_umbrella.h>

namespace yas::ae {
class track_presenter;
class ui_modules;
class ui_markers;
class ui_edge;
class display_space;

struct ui_track final {
    [[nodiscard]] static std::shared_ptr<ui_track> make_shared(window_lifetime_id const &,
                                                               std::shared_ptr<ui::standard> const &,
                                                               std::shared_ptr<display_space> const &,
                                                               std::shared_ptr<ui_modules> const &);

    ui_track(std::shared_ptr<ui::standard> const &, std::shared_ptr<display_space> const &,
             std::shared_ptr<track_presenter> const &, std::shared_ptr<ui_modules> const &);

    std::shared_ptr<ui::node> const node;

   private:
    std::shared_ptr<track_presenter> const _presenter;

    std::shared_ptr<display_space> const _display_space;
    std::shared_ptr<ui_modules> const _modules;

    observing::canceller_pool _pool;

    void _update_scale();
};
}  // namespace yas::ae

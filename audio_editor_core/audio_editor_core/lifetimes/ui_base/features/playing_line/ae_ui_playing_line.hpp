//
//  ae_ui_playing_line.hpp
//

#pragma once

#include <audio_editor_core/ae_window_lifetime_id.h>
#include <ui/yas_ui_umbrella.h>

namespace yas::ae {
class color;
class playing_line_presenter;

struct ui_playing_line final {
    [[nodiscard]] static std::shared_ptr<ui_playing_line> make_shared(window_lifetime_id const &,
                                                                      std::shared_ptr<ui::node> const &);
    ui_playing_line(ae::color *, std::shared_ptr<playing_line_presenter> const &, std::shared_ptr<ui::standard> const &,
                    std::shared_ptr<ui::node> const &);

   private:
    ae::color *const _color;
    std::shared_ptr<playing_line_presenter> const _presenter;
    std::shared_ptr<ui::rect_plane> const _playing_line;
    observing::canceller_pool _pool;

    ui_playing_line(ui_playing_line const &) = delete;
    ui_playing_line(ui_playing_line &&) = delete;
    ui_playing_line &operator=(ui_playing_line const &) = delete;
    ui_playing_line &operator=(ui_playing_line &&) = delete;
};
}  // namespace yas::ae

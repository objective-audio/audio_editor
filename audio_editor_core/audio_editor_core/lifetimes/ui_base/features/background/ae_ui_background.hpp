//
//  ae_ui_background.hpp
//

#pragma once

#include <audio_editor_core/ae_window_lifetime_id.h>
#include <ui/yas_ui_umbrella.h>

namespace yas::ae {
class color;

struct ui_background final {
    [[nodiscard]] static std::shared_ptr<ui_background> make_shared(window_lifetime_id const &);
    ui_background(std::shared_ptr<ui::standard> const &, ae::color *);

   private:
    ui::background *const _background;
    ae::color *const _color;

    observing::canceller_pool _pool;
};
}  // namespace yas::ae

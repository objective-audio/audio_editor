//
//  ae_display_space.h
//

#pragma once

#include <observing/yas_observing_umbrella.h>
#include <ui/yas_ui_types.h>

namespace yas::ae {
struct display_space final {
    [[nodiscard]] static std::shared_ptr<display_space> make_shared(ui::region const view_region);

    void set_view_region(ui::region const);
    void set_scale(ui::size const);

    ui::region region() const;
    observing::syncable observe_region(std::function<void(ui::region const &)> &&);

   private:
    ui::region _view_region;
    ui::size _scale;
    observing::value::holder_ptr<ui::region> const _region;

    observing::canceller_pool _pool;

    display_space(ui::region const view_region);

    void _update();
};
}  // namespace yas::ae

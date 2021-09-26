//
//  ae_ui_button.h
//

#pragma once

#include <ui/yas_ui_umbrella.h>

namespace yas::ae {
struct ui_button {
    void set_text(std::string const &);
    void set_enabled(bool const);

    std::shared_ptr<ui::node> const &node() const;

    observing::endable observe_tapped(std::function<void(void)> &&);

    std::shared_ptr<ui::layout_region_guide> const &layout_guide() const;

    static std::shared_ptr<ui_button> make_shared(std::shared_ptr<ui::font_atlas> const &,
                                                  std::shared_ptr<ui::standard> const &);

   private:
    std::shared_ptr<ui::button> const _button;
    std::shared_ptr<ui::strings> const _strings;
    observing::canceller_pool _pool;

    ui_button(std::shared_ptr<ui::font_atlas> const &, std::shared_ptr<ui::standard> const &);
};
}  // namespace yas::ae

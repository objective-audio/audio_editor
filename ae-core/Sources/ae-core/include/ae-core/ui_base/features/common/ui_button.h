//
//  ui_button.h
//

#pragma once

#include <ui/yas_ui_umbrella.h>

namespace yas::ae {
struct ui_button final {
    ui_button(std::shared_ptr<ui::font_atlas> const &, std::shared_ptr<ui::standard> const &);

    void set_text(std::string const &);
    void set_enabled(bool const);

    [[nodiscard]] std::shared_ptr<ui::node> const &node() const;

    [[nodiscard]] observing::endable observe_tapped(std::function<void(void)> &&);

    [[nodiscard]] std::shared_ptr<ui::layout_region_guide> const &layout_guide() const;

   private:
    std::shared_ptr<ui::button> const _button;
    std::shared_ptr<ui::strings> const _strings;
    observing::canceller_pool _pool;

    ui_button(ui_button const &) = delete;
    ui_button(ui_button &&) = delete;
    ui_button &operator=(ui_button const &) = delete;
    ui_button &operator=(ui_button &&) = delete;
};
}  // namespace yas::ae

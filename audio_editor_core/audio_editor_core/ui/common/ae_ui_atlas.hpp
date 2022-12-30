//
//  ae_ui_atlas.hpp
//

#pragma once

#include <observing/yas_observing_umbrella.h>
#include <ui/yas_ui_texture.h>

namespace yas::ae {
struct ui_atlas final {
    explicit ui_atlas(std::shared_ptr<ui::texture> const &);

    [[nodiscard]] std::shared_ptr<ui::texture> const &texture() const;

    [[nodiscard]] observing::syncable observe_white_filled_tex_coords(
        std::function<void(ui::uint_region const &)> &&) const;

   private:
    std::shared_ptr<ui::texture> const _texture;
    std::shared_ptr<ui::texture_element> _white_filled_element;

    ui_atlas(ui_atlas const &) = delete;
    ui_atlas(ui_atlas &&) = delete;
    ui_atlas &operator=(ui_atlas const &) = delete;
    ui_atlas &operator=(ui_atlas &&) = delete;
};
}  // namespace yas::ae

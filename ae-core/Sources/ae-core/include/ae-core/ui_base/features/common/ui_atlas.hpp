//
//  ui_atlas.hpp
//

#pragma once

#include <ui/texture/yas_ui_texture.h>

#include <observing/umbrella.hpp>

namespace yas::ae {
struct ui_atlas final {
    explicit ui_atlas(std::shared_ptr<ui::texture> const &);

    [[nodiscard]] std::shared_ptr<ui::texture> const &texture() const;

    [[nodiscard]] ui::uint_region const &white_filled_tex_coords() const;
    [[nodiscard]] observing::syncable observe_white_filled_tex_coords(
        std::function<void(ui::uint_region const &)> &&) const;

   private:
    std::shared_ptr<ui::texture> const _texture;
    observing::value::holder_ptr<ui::uint_region> const _tex_coords;

    observing::canceller_pool _pool;

    ui_atlas(ui_atlas const &) = delete;
    ui_atlas(ui_atlas &&) = delete;
    ui_atlas &operator=(ui_atlas const &) = delete;
    ui_atlas &operator=(ui_atlas &&) = delete;
};
}  // namespace yas::ae

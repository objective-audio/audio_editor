//
//  ae_ui_range_selection.hpp
//

#pragma once

#include <audio_editor_core/ae_project_lifetime_id.h>
#include <ui/yas_ui_umbrella.h>

namespace yas::ae {
class ui_square_mesh_data;
class ui_framed_square;
class color;
class range_selector;

struct ui_range_selection final {
    static std::shared_ptr<ui_range_selection> make_shared(project_lifetime_id const &, ui::node *);

    ui_range_selection(ui::node *, ae::color *, ui::standard *, ui_square_mesh_data *,
                       std::shared_ptr<range_selector> const &);

   private:
    ui::node *const _node;
    ae::color *const _color;
    std::shared_ptr<ui_framed_square> const _framed_square;

    std::weak_ptr<range_selector> const _range_selector;

    observing::canceller_pool _pool;

    ui_range_selection(ui_range_selection const &) = delete;
    ui_range_selection(ui_range_selection &&) = delete;
    ui_range_selection &operator=(ui_range_selection const &) = delete;
    ui_range_selection &operator=(ui_range_selection &&) = delete;
};
}  // namespace yas::ae

//
//  ae_ui_range_selection_input.hpp
//

#pragma once

#include <audio_editor_core/ae_window_lifetime_id.h>
#include <ui/yas_ui_umbrella.h>

namespace yas::ae {
class range_selector;

struct ui_range_selection_input final {
    static std::shared_ptr<ui_range_selection_input> make_shared(window_lifetime_id const &, ui::node *);

    ui_range_selection_input(std::shared_ptr<ui::standard> const &, ui::node *,
                             std::shared_ptr<range_selector> const &);

   private:
    std::shared_ptr<ui::node> const _collider_node;
    std::shared_ptr<ui::collider> const _collider;
    std::shared_ptr<ui::touch_tracker> const _touch_tracker;

    std::weak_ptr<range_selector> const _range_selector;

    observing::canceller_pool _pool;

    ui_range_selection_input(ui_range_selection_input const &) = delete;
    ui_range_selection_input(ui_range_selection_input &&) = delete;
    ui_range_selection_input &operator=(ui_range_selection_input const &) = delete;
    ui_range_selection_input &operator=(ui_range_selection_input &&) = delete;
};
}  // namespace yas::ae

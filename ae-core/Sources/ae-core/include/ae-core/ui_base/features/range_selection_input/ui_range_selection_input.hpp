//
//  ui_range_selection_input.hpp
//

#pragma once

#include <ae-core/global/value_types/project_lifetime_id.h>
#include <ui/yas_ui_umbrella.h>

namespace yas::ae {
class range_selector;
class modifiers_holder;
class deselector;

struct ui_range_selection_input final {
    static std::shared_ptr<ui_range_selection_input> make_shared(project_lifetime_id const &, ui::node *);

    ui_range_selection_input(std::shared_ptr<ui::standard> const &, ui::node *, std::shared_ptr<range_selector> const &,
                             std::shared_ptr<deselector> const &, std::shared_ptr<modifiers_holder> const &);

   private:
    std::shared_ptr<ui::node> const _collider_node;
    std::shared_ptr<ui::collider> const _collider;
    std::shared_ptr<ui::touch_tracker> const _touch_tracker;

    std::weak_ptr<range_selector> const _range_selector;
    std::weak_ptr<deselector> const _deselector;
    std::weak_ptr<modifiers_holder> const _modifiers_holder;

    observing::canceller_pool _pool;

    ui_range_selection_input(ui_range_selection_input const &) = delete;
    ui_range_selection_input(ui_range_selection_input &&) = delete;
    ui_range_selection_input &operator=(ui_range_selection_input const &) = delete;
    ui_range_selection_input &operator=(ui_range_selection_input &&) = delete;
};
}  // namespace yas::ae

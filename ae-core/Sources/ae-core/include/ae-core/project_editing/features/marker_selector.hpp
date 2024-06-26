//
//  marker_selector.hpp
//

#pragma once

#include <ae-core/project_editing/features/selected_marker_pool.hpp>

namespace yas::ae {
class marker_pool;
class editing_status;
class deselector;
class selector_enabler;

struct marker_selector final {
    marker_selector(marker_pool const *, selected_marker_pool *, editing_status const *, deselector *,
                    selector_enabler const *);

    [[nodiscard]] bool can_select() const;
    void begin_selection();
    void select(std::set<marker_index> const &);
    void end_selection();

    [[nodiscard]] bool can_toggle() const;
    void toggle(marker_index const &);

   private:
    marker_pool const *const _marker_pool;
    selected_marker_pool *const _selected_pool;
    editing_status const *const _editing_status;
    deselector *const _deselector;
    selector_enabler const *const _enabler;

    marker_selector(marker_selector const &) = delete;
    marker_selector(marker_selector &&) = delete;
    marker_selector &operator=(marker_selector const &) = delete;
    marker_selector &operator=(marker_selector &&) = delete;
};
}  // namespace yas::ae

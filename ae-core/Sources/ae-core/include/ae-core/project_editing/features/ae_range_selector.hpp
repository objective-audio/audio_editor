//
//  ae_range_selector.hpp
//

#pragma once

#include <ae-core/project_editing/features/ae_range_selector_dependency.h>
#include <ae-core/project_editing/features/ae_range_selector_types.h>

#include <ae-core/project_editing/value_types/ae_point_range.hpp>
#include <ae-core/project_editing/value_types/ae_range_selection.hpp>
#include <observing/umbrella.hpp>

namespace yas::ae {
struct range_selector final {
    range_selector(player_for_range_selector *, deselector_for_range_selector *);

    void begin(ui::point const &);
    void move(ui::point const &);
    void end();

    [[nodiscard]] range_selection const &selection() const;
    [[nodiscard]] observing::syncable observe(range_selection_order const,
                                              std::function<void(range_selection const &)> &&) const;

   private:
    observing::value::holder_ptr<range_selection> const _selection;
    player_for_range_selector *_player;
    deselector_for_range_selector *_deselector;

    observing::canceller_pool _pool;

    range_selector(range_selector const &) = delete;
    range_selector(range_selector &&) = delete;
    range_selector &operator=(range_selector const &) = delete;
    range_selector &operator=(range_selector &&) = delete;
};
}  // namespace yas::ae

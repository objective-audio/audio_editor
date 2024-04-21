//
//  ae_track_selector.hpp
//

#pragma once

#include <ae-core/project_editing/features/ae_selected_track_pool.hpp>

namespace yas::ae {
class editing_status;
class deselector;
class selector_enabler;

struct track_selector final {
    track_selector(selected_track_pool *, editing_status const *, deselector *, selector_enabler const *);

    [[nodiscard]] bool can_select() const;
    void begin_selection();
    void select(std::set<track_index_t> const &);
    void end_selection();

    [[nodiscard]] bool can_toggle() const;
    void toggle(track_index_t const &);

   private:
    selected_track_pool *_selected_pool;
    editing_status const *const _editing_status;
    deselector *const _deselector;
    selector_enabler const *const _enabler;

    track_selector(track_selector const &) = delete;
    track_selector(track_selector &&) = delete;
    track_selector &operator=(track_selector const &) = delete;
    track_selector &operator=(track_selector &&) = delete;
};
}  // namespace yas::ae

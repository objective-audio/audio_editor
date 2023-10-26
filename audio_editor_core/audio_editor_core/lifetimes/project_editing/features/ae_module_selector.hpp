//
//  ae_module_selector.hpp
//

#pragma once

#include <audio_editor_core/ae_module_index.hpp>
#include <audio_editor_core/ae_selected_module_pool.hpp>
#include <audio_editor_core/ae_selected_track_pool.hpp>

namespace yas::ae {
class module_pool;
class editing_status;
class deselector;
class selector_enabler;

struct module_selector final {
    module_selector(module_pool const *, selected_module_pool *, selected_track_pool *, editing_status const *,
                    selector_enabler const *);

    [[nodiscard]] bool can_select() const;
    void begin_selection();
    void select(std::set<module_index> const &);
    void end_selection();

    [[nodiscard]] bool can_toggle() const;
    void toggle(module_index const &);

    void select_all();

   private:
    module_pool const *const _module_pool;
    selected_module_pool *const _selected_module_pool;
    selected_track_pool const *const _selected_track_pool;
    editing_status const *const _editing_status;
    selector_enabler const *const _enabler;

    module_selector(module_selector const &) = delete;
    module_selector(module_selector &&) = delete;
    module_selector &operator=(module_selector const &) = delete;
    module_selector &operator=(module_selector &&) = delete;
};
}  // namespace yas::ae

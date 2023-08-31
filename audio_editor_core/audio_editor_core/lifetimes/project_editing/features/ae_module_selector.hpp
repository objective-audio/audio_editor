//
//  ae_module_selector.hpp
//

#pragma once

#include <audio_editor_core/ae_module_index.hpp>
#include <audio_editor_core/ae_selected_module_pool.hpp>

namespace yas::ae {
class module_pool;
class editing_status;
class deselector;

struct module_selector final {
    module_selector(module_pool const *, selected_module_pool *, editing_status const *);

    [[nodiscard]] bool can_select() const;
    void begin_selection();
    void select(std::set<module_index> const &);
    void end_selection();

    [[nodiscard]] bool can_toggle() const;
    void toggle(module_index const &);

   private:
    module_pool const *const _module_pool;
    selected_module_pool *const _selected_pool;
    editing_status const *const _editing_status;

    module_selector(module_selector const &) = delete;
    module_selector(module_selector &&) = delete;
    module_selector &operator=(module_selector const &) = delete;
    module_selector &operator=(module_selector &&) = delete;
};
}  // namespace yas::ae

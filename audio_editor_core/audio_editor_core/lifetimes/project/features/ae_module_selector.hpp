//
//  ae_module_selector.hpp
//

#pragma once

#include <audio_editor_core/ae_file_module_index.hpp>
#include <memory>

namespace yas::ae {
class file_track;
class selected_file_module_pool;
class editing_status;

struct module_selector final {
    module_selector(file_track const *, selected_file_module_pool *, editing_status const *);

    [[nodiscard]] bool can_select() const;
    void select_module_at(file_module_index const &);

    [[nodiscard]] bool can_toggle() const;
    void toggle_module_at(file_module_index const &);

   private:
    file_track const *const _file_track;
    selected_file_module_pool *const _selected_pool;
    editing_status const *const _editing_status;
};
}  // namespace yas::ae

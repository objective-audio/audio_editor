//
//  ae_module_selector.hpp
//

#pragma once

#include <audio_editor_core/ae_file_module_index.hpp>
#include <memory>

namespace yas::ae {
class file_track;
class selected_file_module_pool;

struct module_selector final {
    module_selector(file_track const *, selected_file_module_pool *);

    [[nodiscard]] bool can_toggle() const;
    void toggle_module_at(file_module_index const &);

   private:
    file_track const *const _file_track;
    selected_file_module_pool *const _selected_pool;
};
}  // namespace yas::ae

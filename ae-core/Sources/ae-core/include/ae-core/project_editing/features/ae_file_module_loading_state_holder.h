//
//  ae_file_module_loading_state_holder.h
//

#pragma once

#include <ae-core/project_editing/value_types/ae_file_module_loading_state.h>

#include <observing/umbrella.hpp>

namespace yas::ae {
struct file_module_loading_state_holder final {
    file_module_loading_state_holder();

    [[nodiscard]] file_module_loading_state const &state() const;
    void set_state(file_module_loading_state const);

   private:
    observing::value::holder_ptr<file_module_loading_state> const _state;

    file_module_loading_state_holder(file_module_loading_state_holder const &) = delete;
    file_module_loading_state_holder(file_module_loading_state_holder &&) = delete;
    file_module_loading_state_holder &operator=(file_module_loading_state_holder const &) = delete;
    file_module_loading_state_holder &operator=(file_module_loading_state_holder &&) = delete;
};
}  // namespace yas::ae

//
//  ae_file_module_loading_state_holder.h
//

#pragma once

#include <audio_editor_core/ae_file_module_loading_state.h>
#include <observing/yas_observing_umbrella.h>

namespace yas::ae {
struct file_module_loading_state_holder final {
    [[nodiscard]] static std::shared_ptr<file_module_loading_state_holder> make_shared();

    file_module_loading_state_holder();

    file_module_loading_state const &state() const;
    void set_state(file_module_loading_state const);

   private:
    observing::value::holder_ptr<file_module_loading_state> const _state;
};
}  // namespace yas::ae

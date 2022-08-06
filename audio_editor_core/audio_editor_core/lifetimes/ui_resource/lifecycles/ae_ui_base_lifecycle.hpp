//
//  ae_ui_base_lifecycle.hpp
//

#pragma once

#include <audio_editor_core/ae_window_lifetime_id.h>

#include <memory>

namespace yas::ae {
class ui_base_lifetime;

struct ui_base_lifecycle final {
    ui_base_lifecycle(window_lifetime_id const &);

    window_lifetime_id const window_lifetime_id;

    void add_lifetime();
    std::shared_ptr<ui_base_lifetime> const &lifetime() const;

   private:
    std::shared_ptr<ui_base_lifetime> _lifetime;
};
}  // namespace yas::ae

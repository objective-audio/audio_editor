//
//  ae_ui_root_lifecycle.h
//

#pragma once

#include <audio_editor_core/ae_window_lifetime_id.h>
#include <ui/yas_ui_standard.h>

#include <map>
#include <string>

namespace yas::ae {
class ui_root_lifetime;

struct ui_root_lifecycle {
    [[nodiscard]] static std::shared_ptr<ui_root_lifecycle> make_shared();

    ui_root_lifecycle();

    void add_lifetime(std::shared_ptr<ui::standard> const &, window_lifetime_id const &);
    [[nodiscard]] std::shared_ptr<ui_root_lifetime> const &lifetime_for_window_lifetime_id(
        window_lifetime_id const &) const;
    void remove_lifetime_for_window_lifetime_id(window_lifetime_id const &);

   private:
    std::map<window_lifetime_id, std::shared_ptr<ui_root_lifetime>> _ui_roots;
};
}  // namespace yas::ae

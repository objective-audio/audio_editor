//
//  ae_ui_resource_lifecycle.hpp
//

#pragma once

#include <audio_editor_core/ae_window_lifetime_id.h>
#include <ui/yas_ui_standard.h>

#include <map>
#include <string>

namespace yas::ae {
class ui_resource_lifetime;

struct ui_resource_lifecycle final {
    ui_resource_lifecycle();

    void add_lifetime(std::shared_ptr<ui::standard> const &, window_lifetime_id const &);
    void remove_lifetime_for_window_lifetime_id(window_lifetime_id const &);
    [[nodiscard]] std::shared_ptr<ui_resource_lifetime> const &lifetime_for_window_lifetime_id(
        window_lifetime_id const &) const;

   private:
    std::map<window_lifetime_id, std::shared_ptr<ui_resource_lifetime>> _lifetimes;
};
}  // namespace yas::ae

//
//  ae_context_menu_presenter.h
//

#pragma once

#include <audio_editor_core/ae_context_menu.h>
#include <observing/yas_observing_umbrella.h>

namespace yas::ae {
struct context_menu_presenter final {
    [[nodiscard]] static std::shared_ptr<context_menu_presenter> make_shared();

    std::optional<context_menu> const &context_menu() const;
    void set_context_menu(std::optional<ae::context_menu> const &);

    [[nodiscard]] observing::syncable observe(std::function<void(std::optional<ae::context_menu> const &)> &&);

   private:
    observing::value::holder_ptr<std::optional<ae::context_menu>> const _value;

    context_menu_presenter();
};
}  // namespace yas::ae

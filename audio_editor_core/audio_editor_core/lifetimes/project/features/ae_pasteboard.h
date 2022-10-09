//
//  ae_pasteboard.h
//

#pragma once

#include <audio_editor_core/ae_pasteboard_types.h>
#include <observing/yas_observing_umbrella.h>

namespace yas::ae {
struct pasteboard final {
    [[nodiscard]] static std::shared_ptr<pasteboard> make_shared();

    pasteboard();

    [[nodiscard]] std::vector<pasting_file_module_object> const &file_modules() const;
    void set_file_modules(std::vector<pasting_file_module_object> const &);

    [[nodiscard]] std::optional<pasting_value> const &value() const;

    [[nodiscard]] bool can_clear();
    void clear();

    [[nodiscard]] observing::syncable observe_event(std::function<void(pasteboard_event const &)> &&);

   private:
    std::optional<pasting_value> _value;
    observing::fetcher_ptr<pasteboard_event> const _event_fetcher;
};
}  // namespace yas::ae

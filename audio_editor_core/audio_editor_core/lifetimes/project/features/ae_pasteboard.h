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

    [[nodiscard]] std::optional<pasting_file_module> file_module() const;
    void set_file_module(pasting_file_module const &);

    [[nodiscard]] std::string data() const;
    void revert_data(std::string const &);

    [[nodiscard]] observing::syncable observe_event(std::function<void(pasteboard_event const &)> &&);

   private:
    std::optional<pasting_value> _value;
    observing::fetcher_ptr<pasteboard_event> const _event_fetcher;
};
}  // namespace yas::ae

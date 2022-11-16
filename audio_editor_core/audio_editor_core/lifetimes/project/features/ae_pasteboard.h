//
//  ae_pasteboard.h
//

#pragma once

#include <observing/yas_observing_umbrella.h>

#include <audio_editor_core/ae_pasting_file_module.hpp>
#include <variant>

namespace yas::ae {
enum class pasting_kind {
    file_module,
};

enum class pasteboard_event {
    fetched,
    file_modules,
    cleared,
};

using pasting_value = std::variant<std::vector<pasting_file_module_object>>;

struct pasteboard final {
    [[nodiscard]] static std::shared_ptr<pasteboard> make_shared();

    pasteboard();

    [[nodiscard]] std::vector<pasting_file_module_object> const &file_modules() const;
    void set_file_modules(std::vector<pasting_file_module_object> const &);

    [[nodiscard]] std::optional<pasting_value> const &value() const;

    [[nodiscard]] bool can_clear() const;
    void clear();

    [[nodiscard]] observing::syncable observe_event(std::function<void(pasteboard_event const &)> &&);

   private:
    std::optional<pasting_value> _value;
    observing::fetcher_ptr<pasteboard_event> const _event_fetcher;
};
}  // namespace yas::ae

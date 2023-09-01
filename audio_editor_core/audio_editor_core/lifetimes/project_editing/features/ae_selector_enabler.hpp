//
//  ae_selector_enabler.hpp
//

#pragma once

#include <audio_editor_core/ae_selector_kind.h>

#include <audio_editor_core/ae_selected_marker_pool.hpp>
#include <audio_editor_core/ae_selected_module_pool.hpp>
#include <optional>

namespace yas::ae {
struct selector_enabler {
    selector_enabler(selected_module_pool const *, selected_marker_pool const *);

    bool is_enabled(selector_kind const) const;
    bool is_any_enabled() const;

   private:
    selected_module_pool const *const _module_pool;
    selected_marker_pool const *const _marker_pool;

    selector_enabler(selector_enabler const &) = delete;
    selector_enabler(selector_enabler &&) = delete;
    selector_enabler &operator=(selector_enabler const &) = delete;
    selector_enabler &operator=(selector_enabler &&) = delete;

    std::optional<selector_kind> _current_kind() const;
};
}  // namespace yas::ae

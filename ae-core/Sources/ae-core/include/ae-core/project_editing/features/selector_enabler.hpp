//
//  selector_enabler.hpp
//

#pragma once

#include <ae-core/project_editing/value_types/selector_kind.h>

#include <ae-core/project_editing/features/selected_marker_pool.hpp>
#include <ae-core/project_editing/features/selected_module_pool.hpp>
#include <ae-core/project_editing/features/selected_track_pool.hpp>
#include <optional>

namespace yas::ae {
struct selector_enabler {
    selector_enabler(selected_module_pool const *, selected_marker_pool const *, selected_track_pool const *);

    bool is_enabled(selector_kind const) const;
    bool is_any_enabled() const;
    std::optional<selector_kind> const current_kind() const;

   private:
    selected_module_pool const *const _module_pool;
    selected_marker_pool const *const _marker_pool;
    selected_track_pool const *const _track_pool;

    selector_enabler(selector_enabler const &) = delete;
    selector_enabler(selector_enabler &&) = delete;
    selector_enabler &operator=(selector_enabler const &) = delete;
    selector_enabler &operator=(selector_enabler &&) = delete;
};
}  // namespace yas::ae

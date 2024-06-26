//
//  deselector.hpp
//

#pragma once

#include <ae-core/project_editing/features/range_selector_dependency.h>

#include <ae-core/project_editing/features/selected_marker_pool.hpp>
#include <ae-core/project_editing/features/selected_module_pool.hpp>
#include <ae-core/project_editing/features/selected_track_pool.hpp>

namespace yas::ae {
struct deselector final : deselector_for_range_selector {
    deselector(selected_module_pool *, selected_marker_pool *, selected_track_pool *);

    [[nodiscard]] bool can_deselect() const;
    void deselect_all();

   private:
    selected_module_pool *const _selected_module_pool;
    selected_marker_pool *const _selected_marker_pool;
    selected_track_pool *const _selected_track_pool;

    deselector(deselector const &) = delete;
    deselector(deselector &&) = delete;
    deselector &operator=(deselector const &) = delete;
    deselector &operator=(deselector &&) = delete;
};
}  // namespace yas::ae

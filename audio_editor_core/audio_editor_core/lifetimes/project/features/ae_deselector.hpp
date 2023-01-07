//
//  ae_deselector.hpp
//

#pragma once

namespace yas::ae {
class selected_module_pool;
class selected_marker_pool;

struct deselector final {
    deselector(selected_module_pool *, selected_marker_pool *);

    void deselect_all();

   private:
    selected_module_pool *const _selected_module_pool;
    selected_marker_pool *const _selected_marker_pool;

    deselector(deselector const &) = delete;
    deselector(deselector &&) = delete;
    deselector &operator=(deselector const &) = delete;
    deselector &operator=(deselector &&) = delete;
};
}  // namespace yas::ae
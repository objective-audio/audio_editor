//
//  ae_deselector.hpp
//

#pragma once

namespace yas::ae {
class selected_file_module_pool;
class selected_marker_pool;

struct deselector final {
    deselector(selected_file_module_pool *, selected_marker_pool *);

    void deselect_all();

   private:
    selected_file_module_pool *const _selected_file_module_pool;
    selected_marker_pool *const _selected_marker_pool;
};
}  // namespace yas::ae

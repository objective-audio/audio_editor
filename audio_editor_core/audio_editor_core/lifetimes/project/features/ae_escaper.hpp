//
//  ae_escaper.hpp
//

#pragma once

namespace yas::ae {
class pasteboard;
class selected_file_module_pool;

struct escaper final {
    escaper(pasteboard *, selected_file_module_pool *);

    [[nodiscard]] bool can_escape() const;
    void espace();

   private:
    pasteboard *const _pasteboard;
    selected_file_module_pool *const _selected_pool;
};
}  // namespace yas::ae
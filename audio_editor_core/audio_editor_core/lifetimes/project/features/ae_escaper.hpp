//
//  ae_escaper.hpp
//

#pragma once

#include <audio_editor_core/ae_selected_marker_pool.hpp>
#include <audio_editor_core/ae_selected_module_pool.hpp>

namespace yas::ae {
class pasteboard;

struct escaper final {
    escaper(pasteboard *, selected_module_pool *, selected_marker_pool *);

    [[nodiscard]] bool can_escape() const;
    void espace();

   private:
    pasteboard *const _pasteboard;
    selected_module_pool *const _selected_module_pool;
    selected_marker_pool *const _selected_marker_pool;

    escaper(escaper const &) = delete;
    escaper(escaper &&) = delete;
    escaper &operator=(escaper const &) = delete;
    escaper &operator=(escaper &&) = delete;
};
}  // namespace yas::ae

//
//  ae_db_edge.h
//

#pragma once

#include <audio_editor_core/ae_edge.h>

#include <db/yas_db_umbrella.hpp>

namespace yas::ae {
struct db_edge final {
    explicit db_edge(db::object_ptr const &);

    [[nodiscard]] static db_edge create(db::manager_ptr const &, edge const &);

    [[nodiscard]] ae::edge edge() const;

    void set(ae::edge const &);

   private:
    db::object_ptr const _object;
};
}  // namespace yas::ae

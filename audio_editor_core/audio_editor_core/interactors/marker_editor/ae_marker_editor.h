//
//  ae_marker_editor.h
//

#pragma once

#include <audio_editor_core/ae_project_id.h>

#include <memory>

namespace yas::ae {
class player;
class marker_pool;
class database;
class exporter;

struct marker_editor final {
    [[nodiscard]] static std::shared_ptr<marker_editor> make_shared(project_id const &, marker_pool *, database *,
                                                                    exporter const *);

    [[nodiscard]] bool can_insert_marker() const;
    void insert_marker();

   private:
    player const *const _player;
    marker_pool *const _marker_pool;
    database *const _database;
    exporter const *const _exporter;

    marker_editor(player const *, marker_pool *, database *, exporter const *);

    bool _can_editing() const;
};
}  // namespace yas::ae

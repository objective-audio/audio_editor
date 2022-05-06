//
//  ae_edge_editor.h
//

#pragma once

#include <audio_editor_core/ae_project_id.h>

#include <memory>

namespace yas::ae {
class edge_holder;
class player;
class exporter;

struct edge_editor final {
    [[nodiscard]] static std::shared_ptr<edge_editor> make_shared(project_id const &project_id, edge_holder *,
                                                                  exporter const *);

    [[nodiscard]] bool can_set_begin() const;
    [[nodiscard]] bool can_set_end() const;
    void set_begin();
    void set_end();

   private:
    edge_holder *const _holder;
    player const *const _player;
    exporter const *const _exporter;

    edge_editor(edge_holder *, player const *, exporter const *);

    bool _can_editing() const;
};
}  // namespace yas::ae

//
//  ae_edge_editor.h
//

#pragma once

#include <audio_editor_core/ae_edge_editor_types.h>
#include <audio_editor_core/ae_project_editor_dependency.h>
#include <observing/yas_observing_umbrella.h>

namespace yas::ae {
struct edge_editor final : edge_editor_for_project_editor {
    [[nodiscard]] static std::shared_ptr<edge_editor> make_shared();

    [[nodiscard]] ae::edge const &edge() const override;
    void set_edge(ae::edge const &) override;
    void set_begin_frame(frame_index_t const) override;
    void set_end_frame(frame_index_t const) override;
    void revert_edge(ae::edge const &) override;

    [[nodiscard]] observing::syncable observe_event(std::function<void(edge_editor_event const &)> &&) override;

   private:
    ae::edge _edge;

    observing::fetcher_ptr<edge_editor_event> _fetcher;

    edge_editor();

    void _set_edge_and_notify(ae::edge const &, edge_editor_event_type const);
};
}  // namespace yas::ae

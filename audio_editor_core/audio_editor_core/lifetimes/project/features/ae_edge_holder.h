//
//  ae_edge_holder.h
//

#pragma once

#include <audio_editor_core/ae_edge_holder_types.h>
#include <audio_editor_core/ae_jumper_dependency.h>
#include <observing/yas_observing_umbrella.h>

namespace yas::ae {
struct edge_holder final : jumpable_on_jumper {
    edge_holder();

    [[nodiscard]] ae::edge const &edge() const;
    void set_edge(ae::edge const &);
    void set_begin_frame(frame_index_t const);
    void set_end_frame(frame_index_t const);
    void revert_edge(ae::edge const &);

    [[nodiscard]] std::optional<frame_index_t> next_jumpable_frame(frame_index_t const) const override;
    [[nodiscard]] std::optional<frame_index_t> previous_jumpable_frame(frame_index_t const) const override;

    [[nodiscard]] observing::syncable observe_event(std::function<void(edge_holder_event const &)> &&);

   private:
    ae::edge _edge;

    observing::fetcher_ptr<edge_holder_event> _fetcher;

    void _set_edge_and_notify(ae::edge const &, edge_holder_event_type const);
};
}  // namespace yas::ae

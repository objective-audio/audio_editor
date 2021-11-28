//
//  ae_marker_pool.h
//

#pragma once

#include <audio_editor_core/ae_marker_pool_types.h>
#include <audio_editor_core/ae_project_editor_dependency.h>
#include <observing/yas_observing_umbrella.h>

namespace yas::ae {
struct marker_pool : marker_pool_for_project_editor {
    void revert_markers(std::vector<marker> &&) override;
    void insert_marker(marker const &) override;
    void erase_at(proc::frame_index_t const) override;
    void erase_marker(marker const &) override;
    void erase_range(proc::time::range const) override;

    void move_at(proc::frame_index_t const frame, proc::frame_index_t const new_frame) override;
    void move_offset_from(proc::frame_index_t const from, proc::frame_index_t const offset) override;

    marker_map_t const &markers() const override;
    std::optional<marker> marker_at(std::size_t const) const override;

    std::optional<proc::frame_index_t> next_edge(proc::frame_index_t const) const override;
    std::optional<proc::frame_index_t> previous_edge(proc::frame_index_t const) const override;

    observing::syncable observe_event(std::function<void(marker_pool_event const &)> &&) override;

    static std::shared_ptr<marker_pool> make_shared();

   private:
    observing::map::holder_ptr<proc::frame_index_t, marker> const _markers;
    observing::fetcher_ptr<marker_pool_event> _fetcher;
    observing::canceller_pool _pool;

    marker_pool();
};
}  // namespace yas::ae

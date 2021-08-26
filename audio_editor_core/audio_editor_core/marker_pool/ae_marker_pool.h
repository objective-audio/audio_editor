//
//  ae_marker_pool.h
//

#pragma once

#include <audio_editor_core/ae_marker_pool_types.h>
#include <audio_editor_core/ae_project_editor_dependency.h>
#include <observing/yas_observing_umbrella.h>

namespace yas::ae {
struct marker_pool : marker_pool_for_project_editor {
    void replace_markers(std::vector<marker> &&) override;
    void insert_marker(marker const &) override;
    void erase_at(proc::frame_index_t const) override;
    void erase_marker(marker const &) override;

    std::map<proc::frame_index_t, marker> const &markers() const override;

    std::optional<proc::frame_index_t> next_edge(proc::frame_index_t const) const override;
    std::optional<proc::frame_index_t> previous_edge(proc::frame_index_t const) const override;

    observing::syncable observe_event(std::function<void(marker_pool_event const &)> &&) override;

    static std::shared_ptr<marker_pool> make_shared();

   private:
    observing::map::holder_ptr<proc::frame_index_t, marker> const _markers;

    marker_pool();
};
}  // namespace yas::ae

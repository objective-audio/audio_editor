//
//  ae_marker_pool.h
//

#pragma once

#include <audio_editor_core/ae_marker_pool_types.h>
#include <audio_editor_core/ae_project_editor_dependency.h>
#include <observing/yas_observing_umbrella.h>

namespace yas::ae {
struct marker_pool : marker_pool_for_project_editor {
    void replace_markers(std::vector<marker> &&);
    void insert_marker(marker const &);
    void erase_at(proc::frame_index_t const);
    void erase_marker(marker const &);

    std::map<proc::frame_index_t, marker> const &markers() const;

    observing::syncable observe(std::function<void(marker_pool_event const &)> &&);

    static std::shared_ptr<marker_pool> make_shared();

   private:
    observing::map::holder_ptr<proc::frame_index_t, marker> const _markers;

    marker_pool();
};
}  // namespace yas::ae

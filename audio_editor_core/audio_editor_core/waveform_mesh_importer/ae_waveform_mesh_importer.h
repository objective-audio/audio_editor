//
//  ae_waveform_mesh_importer.h
//

#pragma once

#include <audio_editor_core/ae_module_location.h>
#include <cpp_utils/yas_task_queue.h>
#include <observing/yas_observing_umbrella.h>
#include <ui/yas_ui_umbrella.h>

namespace yas::ae {
struct waveform_mesh_importer final {
    struct event {
        std::size_t index;
        identifier identifier;
        std::shared_ptr<ui::static_mesh_vertex_data> mesh_vertex_data;
        std::shared_ptr<ui::static_mesh_index_data> mesh_index_data;
    };

    [[nodiscard]] static std::shared_ptr<waveform_mesh_importer> make_shared();

    void import(std::size_t const, module_location const &);
    void cancel(identifier const &);
    void cancel_all();

    observing::endable observe(std::function<void(event const &)> &&);

   private:
    std::weak_ptr<waveform_mesh_importer> _weak_importer;
    observing::notifier_ptr<event> const _notifier;
    std::shared_ptr<task_queue<identifier>> const _task_queue;

    waveform_mesh_importer();
};
}  // namespace yas::ae

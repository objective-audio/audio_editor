//
//  ae_waveform_mesh_importer.h
//

#pragma once

#include <audio_editor_core/ae_module_location.h>
#include <audio_editor_core/ae_waveform_mesh_importer_types.h>
#include <cpp_utils/yas_task_queue.h>
#include <cpp_utils/yas_url.h>
#include <observing/yas_observing_umbrella.h>

namespace yas::ae {
class project_url;

struct waveform_mesh_importer final : std::enable_shared_from_this<waveform_mesh_importer> {
    using event = waveform_mesh_importer_event;

    [[nodiscard]] static std::shared_ptr<waveform_mesh_importer> make_shared(project_url const *);

    waveform_mesh_importer(project_url const *);

    void import(std::size_t const, module_location const &);
    void cancel(identifier const &);
    void cancel_all();

    observing::endable observe(std::function<void(event const &)> &&);

   private:
    project_url const *const _project_url;

    observing::notifier_ptr<event> const _notifier;
    std::shared_ptr<task_queue<identifier>> const _task_queue;
};
}  // namespace yas::ae

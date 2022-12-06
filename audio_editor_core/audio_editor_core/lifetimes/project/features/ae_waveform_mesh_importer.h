//
//  ae_waveform_mesh_importer.h
//

#pragma once

#include <audio_editor_core/ae_module_content.h>
#include <audio_editor_core/ae_waveform_mesh_importer_types.h>
#include <audio_editor_core/ae_window_lifetime_id.h>
#include <cpp_utils/yas_task_queue.h>
#include <observing/yas_observing_umbrella.h>

namespace yas::ae {
class project_path;
class file_track;

struct waveform_mesh_importer final : std::enable_shared_from_this<waveform_mesh_importer> {
    using event = waveform_mesh_importer_event;

    [[nodiscard]] static std::shared_ptr<waveform_mesh_importer> make_shared(window_lifetime_id const &, file_track *);

    waveform_mesh_importer(project_path const *, file_track *);

    void import(std::size_t const, module_content const &);
    void cancel(object_id const &);
    void cancel_all();

    [[nodiscard]] observing::endable observe(std::function<void(event const &)> &&);

   private:
    project_path const *const _project_path;
    file_track const *const _file_track;

    observing::notifier_ptr<event> const _notifier;
    std::shared_ptr<task_queue<object_id>> const _task_queue;

    waveform_mesh_importer(waveform_mesh_importer const &) = delete;
    waveform_mesh_importer(waveform_mesh_importer &&) = delete;
    waveform_mesh_importer &operator=(waveform_mesh_importer const &) = delete;
    waveform_mesh_importer &operator=(waveform_mesh_importer &&) = delete;
};
}  // namespace yas::ae

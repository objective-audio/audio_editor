//
//  ae_waveform_mesh_importer.h
//

#pragma once

#include <ae-core/global/value_types/ae_project_lifetime_id.h>
#include <ae-core/project_editing/value_types/ae_module_content.h>
#include <ae-core/project_editing/value_types/ae_waveform_mesh_importer_types.h>
#include <cpp-utils/task_queue.h>

#include <observing/umbrella.hpp>

namespace yas::ae {
class project_path;
class module_pool;

struct waveform_mesh_importer final : std::enable_shared_from_this<waveform_mesh_importer> {
    using event = waveform_mesh_importer_event;

    [[nodiscard]] static std::shared_ptr<waveform_mesh_importer> make_shared(project_lifetime_id const &,
                                                                             module_pool *);

    waveform_mesh_importer(project_path const *, module_pool *);

    void import(std::size_t const, module_content const &);
    void cancel(object_id const &);
    void cancel_all();

    [[nodiscard]] observing::endable observe(std::function<void(event const &)> &&);

   private:
    project_path const *const _project_path;
    module_pool const *const _module_pool;

    observing::notifier_ptr<event> const _notifier;
    std::shared_ptr<task_queue<object_id>> const _task_queue;

    waveform_mesh_importer(waveform_mesh_importer const &) = delete;
    waveform_mesh_importer(waveform_mesh_importer &&) = delete;
    waveform_mesh_importer &operator=(waveform_mesh_importer const &) = delete;
    waveform_mesh_importer &operator=(waveform_mesh_importer &&) = delete;
};
}  // namespace yas::ae

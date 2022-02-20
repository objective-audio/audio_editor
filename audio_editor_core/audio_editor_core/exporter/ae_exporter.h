//
//  ae_exporter.h
//

#pragma once

#include <audio_editor_core/ae_project_editor_dependency.h>
#include <cpp_utils/yas_task_queue.h>

namespace yas::ae {
struct exporter final : exporter_for_project_editor {
    [[nodiscard]] static std::shared_ptr<exporter> make_shared();

    void begin(url const &export_url, std::shared_ptr<proc::timeline> const &, exporting_format const &,
               time::range const &) override;

    [[nodiscard]] bool is_exporting() const override;
    [[nodiscard]] observing::syncable observe_is_exporting(std::function<void(bool const &)> &&) override;

    [[nodiscard]] exporting_result last_result() const;

   private:
    std::weak_ptr<exporter> _weak_exporter;

    exporting_result _last_result;
    observing::value::holder_ptr<bool> const _is_exporting;

    std::shared_ptr<task_queue<std::nullptr_t>> const _queue;

    exporter();
};
}  // namespace yas::ae

//
//  ae_exporter.h
//

#pragma once

#include <audio_editor_core/ae_common_types.h>
#include <audio_editor_core/ae_exporter_types.h>

#include <filesystem>

namespace yas::ae {
struct exporter final : std::enable_shared_from_this<exporter> {
    [[nodiscard]] static std::shared_ptr<exporter> make_shared();

    exporter();

    void begin(std::filesystem::path const &export_path, std::shared_ptr<proc::timeline> const &,
               exporting_format const &, time::range const &);

    [[nodiscard]] bool is_exporting() const;
    [[nodiscard]] observing::syncable observe_is_exporting(std::function<void(bool const &)> &&);

    [[nodiscard]] exporting_result last_result() const;

   private:
    exporting_result _last_result;
    observing::value::holder_ptr<bool> const _is_exporting;

    std::shared_ptr<task_queue<std::nullptr_t>> const _queue;
};
}  // namespace yas::ae

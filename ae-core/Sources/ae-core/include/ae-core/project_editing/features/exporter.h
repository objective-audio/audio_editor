//
//  exporter.h
//

#pragma once

#include <ae-core/global/value_types/common_types.h>
#include <ae-core/project_editing/value_types/exporter_types.h>

#include <filesystem>

namespace yas::ae {
struct exporter final : std::enable_shared_from_this<exporter> {
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

    exporter(exporter const &) = delete;
    exporter(exporter &&) = delete;
    exporter &operator=(exporter const &) = delete;
    exporter &operator=(exporter &&) = delete;
};
}  // namespace yas::ae

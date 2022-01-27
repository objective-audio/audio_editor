//
//  ae_time_presenter.h
//

#pragma once

#include <audio_editor_core/ae_index_range.h>
#include <audio_editor_core/ae_time_presenter_dependency.h>

#include <memory>
#include <optional>
#include <string>

namespace yas::ae {
struct time_presenter final {
    [[nodiscard]] static std::shared_ptr<time_presenter> make_shared(std::string const project_id);

    [[nodiscard]] std::string time_text() const;
    [[nodiscard]] std::optional<index_range> editing_time_text_range() const;

    [[nodiscard]] observing::syncable observe_editing_time_text_range(
        std::function<void(std::optional<index_range> const &)> &&);

   private:
    std::weak_ptr<project_editor_for_time_presenter> _project_editor;
    std::weak_ptr<timing_for_time_presenter> _timing;
    std::weak_ptr<time_editor_for_time_presenter> _time_editor;

    observing::fetcher_ptr<std::optional<index_range>> _range_fetcher;

    observing::canceller_pool _pool;

    time_presenter(std::shared_ptr<project_editor_for_time_presenter> const &,
                   std::shared_ptr<timing_for_time_presenter> const &);
};
}  // namespace yas::ae

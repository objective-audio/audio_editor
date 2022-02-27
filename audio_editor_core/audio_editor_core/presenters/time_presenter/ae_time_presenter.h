//
//  ae_time_presenter.h
//

#pragma once

#include <audio_editor_core/ae_time_presenter_dependency.h>
#include <cpp_utils/yas_index_range.h>

#include <memory>
#include <optional>
#include <string>

namespace yas::ae {
class project_editor;
class timing;

struct time_presenter final {
    [[nodiscard]] static std::shared_ptr<time_presenter> make_shared(std::string const project_id);

    [[nodiscard]] std::string time_text() const;
    [[nodiscard]] std::vector<index_range> time_text_unit_ranges() const;
    [[nodiscard]] std::optional<std::size_t> editing_unit_index() const;
    [[nodiscard]] std::optional<index_range> editing_time_text_range() const;
    [[nodiscard]] observing::syncable observe_editing_time_text_range(
        std::function<void(std::optional<index_range> const &)> &&);

    [[nodiscard]] std::optional<std::size_t> nudging_unit_index() const;
    [[nodiscard]] observing::syncable observe_nudging_unit_index(std::function<void(std::size_t const &)> &&);

   private:
    std::weak_ptr<project_editor> _project_editor;
    std::weak_ptr<timing> _timing;
    std::weak_ptr<time_editor_for_time_presenter> _time_editor;

    observing::fetcher_ptr<std::optional<index_range>> _range_fetcher;

    observing::canceller_pool _pool;

    time_presenter(std::shared_ptr<project_editor> const &, std::shared_ptr<timing> const &);
};
}  // namespace yas::ae

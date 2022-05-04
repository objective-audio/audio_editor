//
//  ae_time_presenter.h
//

#pragma once

#include <cpp_utils/yas_index_range.h>
#include <observing/yas_observing_umbrella.h>

namespace yas::ae {
class timing;
class player;
class nudge_settings;
class time_editor_level;
class time_editor_level_pool;

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
    std::weak_ptr<timing> const _timing;
    std::weak_ptr<player> const _player;
    std::weak_ptr<nudge_settings> const _nudge_settings;
    std::weak_ptr<time_editor_level_pool> const _time_editor_level_pool;

    observing::fetcher_ptr<std::optional<index_range>> _range_fetcher;

    observing::canceller_pool _pool;

    time_presenter(std::shared_ptr<timing> const &, std::shared_ptr<player> const &,
                   std::shared_ptr<nudge_settings> const &, std::shared_ptr<time_editor_level_pool> const &);

    std::shared_ptr<time_editor_level> const &_level() const;
};
}  // namespace yas::ae

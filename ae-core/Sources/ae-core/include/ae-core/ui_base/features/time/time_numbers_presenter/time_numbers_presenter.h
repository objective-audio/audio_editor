//
//  time_numbers_presenter.h
//

#pragma once

#include <ae-core/global/value_types/project_lifetime_id.h>
#include <cpp-utils/index_range.h>

#include <observing/umbrella.hpp>

namespace yas::ae {
class timing;
class player;
class time_editing_lifetime;
class project_modal_lifecycle;

struct time_numbers_presenter final {
    [[nodiscard]] static std::shared_ptr<time_numbers_presenter> make_shared(project_lifetime_id const &);

    time_numbers_presenter(std::shared_ptr<timing> const &, std::shared_ptr<player> const &,
                           std::shared_ptr<project_modal_lifecycle> const &);

    [[nodiscard]] std::string time_text() const;
    [[nodiscard]] std::vector<index_range> time_text_unit_ranges() const;
    [[nodiscard]] std::optional<std::size_t> editing_unit_index() const;
    [[nodiscard]] std::optional<index_range> editing_time_text_range() const;
    [[nodiscard]] observing::syncable observe_editing_time_text_range(
        std::function<void(std::optional<index_range> const &)> &&);

   private:
    std::weak_ptr<timing> const _timing;
    std::weak_ptr<player> const _player;
    std::weak_ptr<project_modal_lifecycle> const _project_modal_lifecycle;

    observing::fetcher_ptr<std::optional<index_range>> _range_fetcher;

    observing::canceller_pool _pool;

    time_numbers_presenter(time_numbers_presenter const &) = delete;
    time_numbers_presenter(time_numbers_presenter &&) = delete;
    time_numbers_presenter &operator=(time_numbers_presenter const &) = delete;
    time_numbers_presenter &operator=(time_numbers_presenter &&) = delete;

    std::shared_ptr<time_editing_lifetime> const &_lifetime() const;
};
}  // namespace yas::ae

//
//  ae_tracks_presenter.hpp
//

#pragma once

#include <audio_editor_core/ae_project_lifetime_id.h>
#include <audio_editor_core/ae_track_content_pool.h>

#include <audio_editor_core/ae_range_selection.hpp>
#include <audio_editor_core/ae_selected_track_pool.hpp>
#include <audio_editor_core/ae_space_range.hpp>

namespace yas::ae {
class display_space;
class display_space_range;
class range_selector;

struct tracks_presenter final {
    [[nodiscard]] static std::shared_ptr<tracks_presenter> make_shared(project_lifetime_id const &);

    tracks_presenter(project_lifetime_id const &, std::shared_ptr<track_content_pool> const &,
                     std::shared_ptr<selected_track_pool> const &, std::shared_ptr<display_space> const &,
                     std::shared_ptr<display_space_range> const &, std::shared_ptr<range_selector> const &);

    [[nodiscard]] std::vector<std::optional<track_content>> const &contents() const;
    [[nodiscard]] observing::syncable observe_contents(std::function<void(track_content_pool_event const &)> &&);
    [[nodiscard]] observing::syncable observe_range(std::function<void(range_selection const &)> &&);

    void update_if_needed();

   private:
    std::weak_ptr<track_content_pool> const _content_pool;
    std::weak_ptr<selected_track_pool> const _selected_pool;
    std::weak_ptr<display_space> const _display_space;
    std::weak_ptr<display_space_range> const _display_space_range;
    std::weak_ptr<range_selector> const _range_selector;
    observing::canceller_pool _canceller_pool;

    std::optional<space_range> _last_space_range = std::nullopt;

    std::optional<ae::space_range> _space_range() const;
    void _replace_contents(selected_track_set const &);
    void _update_all_contents(bool const force_updating, bool const force_replacing);
};
}  // namespace yas::ae

//
//  ae_pasting_modules_presenter.hpp
//

#pragma once

#include <audio_editor_core/ae_common_types.h>
#include <audio_editor_core/ae_pasting_module_location_pool.h>
#include <audio_editor_core/ae_project_format.h>
#include <audio_editor_core/ae_window_lifetime_id.h>

#include <optional>

namespace yas::ae {
class display_space;
class pasteboard;

struct pasting_modules_presenter final {
    [[nodiscard]] static std::shared_ptr<pasting_modules_presenter> make_shared(window_lifetime_id const &,
                                                                                std::shared_ptr<display_space> const &);

    pasting_modules_presenter(project_format const &, std::shared_ptr<pasteboard> const &,
                              std::shared_ptr<display_space> const &,
                              std::shared_ptr<pasting_module_location_pool> const &);

    [[nodiscard]] std::vector<std::optional<pasting_module_location>> const &locations() const;
    [[nodiscard]] observing::syncable observe_locations(
        std::function<void(pasting_module_location_pool_event const &)> &&);

    void update_if_needed();

   private:
    project_format const _project_format;
    std::weak_ptr<pasteboard> const _pasteboard;
    std::weak_ptr<display_space> const _display_space;
    std::weak_ptr<pasting_module_location_pool> const _location_pool;
    observing::canceller_pool _canceller_pool;

    std::optional<frame_index_t> _last_frame = std::nullopt;
    std::optional<time::range> _last_space_range = std::nullopt;

    std::optional<time::range> _space_range() const;
    void _update_all_locations(bool const force_updating, bool const force_replacing);
};
}  // namespace yas::ae

//
//  ae_modules_presenter.h
//

#pragma once

#include <audio_editor_core/ae_module_location.h>
#include <audio_editor_core/ae_module_location_types.h>
#include <audio_editor_core/ae_modules_presenter_dependency.h>

namespace yas::ae {
class module_location_pool;
class display_space;

struct modules_presenter {
    [[nodiscard]] static std::shared_ptr<modules_presenter> make_shared(std::string const &project_id,
                                                                        std::shared_ptr<display_space> const &);
    [[nodiscard]] static std::shared_ptr<modules_presenter> make_shared(
        std::shared_ptr<project_editor_for_modules_presenter> const &, std::shared_ptr<display_space> const &);

    [[nodiscard]] std::vector<std::optional<module_location>> const &locations() const;
    [[nodiscard]] observing::syncable observe_locations(std::function<void(module_location_pool_event const &)> &&);

    void update_if_needed();

   private:
    std::weak_ptr<project_editor_for_modules_presenter> _project_editor;
    std::shared_ptr<display_space> const _display_space;
    std::shared_ptr<module_location_pool> _location_pool;
    observing::canceller_pool _canceller_pool;

    std::optional<proc::frame_index_t> _last_frame = std::nullopt;
    std::optional<proc::time::range> _last_space_range = std::nullopt;

    modules_presenter(std::shared_ptr<project_editor_for_modules_presenter> const &,
                      std::shared_ptr<display_space> const &);

    modules_presenter(modules_presenter const &) = delete;
    modules_presenter(modules_presenter &&) = delete;
    modules_presenter &operator=(modules_presenter const &) = delete;
    modules_presenter &operator=(modules_presenter &&) = delete;

    std::optional<proc::time::range> _space_range() const;
    void _update_all(bool const force);
};
}  // namespace yas::ae

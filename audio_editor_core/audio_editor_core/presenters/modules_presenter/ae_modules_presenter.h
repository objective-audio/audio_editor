//
//  ae_modules_presenter.h
//

#pragma once

#include <audio_editor_core/ae_module_location_pool.h>
#include <audio_editor_core/ae_modules_presenter_dependency.h>
#include <audio_editor_core/ae_project_format.h>
#include <audio_editor_core/ae_window_lifetime_id.h>

#include <audio_editor_core/ae_file_module_index.hpp>

namespace yas::ae {
class display_space;
class player;
class file_track;

struct modules_presenter final {
    [[nodiscard]] static std::shared_ptr<modules_presenter> make_shared(window_lifetime_id const &,
                                                                        std::shared_ptr<display_space> const &);

    modules_presenter(project_format const &, std::shared_ptr<player> const &, std::shared_ptr<file_track> const &,
                      std::shared_ptr<display_space> const &, std::shared_ptr<module_location_pool> const &);

    [[nodiscard]] std::vector<std::optional<module_location>> const &locations() const;
    [[nodiscard]] observing::syncable observe_locations(std::function<void(module_location_pool_event const &)> &&);

    std::string const &name_for_index(file_module_index const &);

    void update_if_needed();

   private:
    project_format const _project_format;
    std::weak_ptr<player> const _player;
    std::weak_ptr<file_track> const _file_track;
    std::weak_ptr<display_space> const _display_space;
    std::weak_ptr<module_location_pool> const _location_pool;
    observing::canceller_pool _canceller_pool;

    std::optional<frame_index_t> _last_frame = std::nullopt;
    std::optional<time::range> _last_space_range = std::nullopt;

    modules_presenter(modules_presenter const &) = delete;
    modules_presenter(modules_presenter &&) = delete;
    modules_presenter &operator=(modules_presenter const &) = delete;
    modules_presenter &operator=(modules_presenter &&) = delete;

    std::optional<time::range> _space_range() const;
    void _update_all_locations(bool const force_updating, bool const force_replacing);
};
}  // namespace yas::ae

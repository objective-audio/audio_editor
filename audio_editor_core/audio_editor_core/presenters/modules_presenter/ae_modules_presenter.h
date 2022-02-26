//
//  ae_modules_presenter.h
//

#pragma once

#include <audio_editor_core/ae_file_info.h>
#include <audio_editor_core/ae_module_location_pool.h>
#include <audio_editor_core/ae_modules_presenter_dependency.h>

namespace yas::ae {
class display_space;

struct modules_presenter final {
    [[nodiscard]] static std::shared_ptr<modules_presenter> make_shared(std::string const &project_id,
                                                                        std::shared_ptr<display_space> const &,
                                                                        std::shared_ptr<module_location_pool> const &);
    [[nodiscard]] static std::shared_ptr<modules_presenter> make_shared(
        file_info const &, std::shared_ptr<project_editor_for_modules_presenter> const &,
        std::shared_ptr<display_space> const &, std::shared_ptr<module_location_pool> const &);

    [[nodiscard]] std::vector<std::optional<module_location>> const &locations() const;
    [[nodiscard]] observing::syncable observe_locations(std::function<void(module_location_pool_event const &)> &&);

    void update_if_needed();

   private:
    file_info const _file_info;
    std::weak_ptr<project_editor_for_modules_presenter> _project_editor;
    std::shared_ptr<display_space> const _display_space;
    std::shared_ptr<module_location_pool> _location_pool;
    observing::canceller_pool _canceller_pool;

    std::optional<frame_index_t> _last_frame = std::nullopt;
    std::optional<time::range> _last_space_range = std::nullopt;

    modules_presenter(file_info const &, std::shared_ptr<project_editor_for_modules_presenter> const &,
                      std::shared_ptr<display_space> const &, std::shared_ptr<module_location_pool> const &);

    modules_presenter(modules_presenter const &) = delete;
    modules_presenter(modules_presenter &&) = delete;
    modules_presenter &operator=(modules_presenter const &) = delete;
    modules_presenter &operator=(modules_presenter &&) = delete;

    std::optional<time::range> _space_range() const;
    void _update_all_locations(bool const force);
};
}  // namespace yas::ae

//
//  ae_track_presenter.h
//

#pragma once

#include <audio_editor_core/ae_file_loader_types.h>
#include <audio_editor_core/ae_file_track_types.h>
#include <observing/yas_observing_umbrella.h>

#include <memory>

namespace yas::ae {
class project;

struct track_presenter {
    [[nodiscard]] static std::shared_ptr<track_presenter> make_shared(std::string const &project_id);
    [[nodiscard]] static std::shared_ptr<track_presenter> make_shared(std::shared_ptr<project> const &);

    [[nodiscard]] file_track_module_map_t const &modules() const;
    [[nodiscard]] std::optional<file_info> file_info() const;
    [[nodiscard]] double current_time() const;

    [[nodiscard]] observing::syncable observe(std::function<void(std::nullptr_t const &)> &&);

   private:
    std::weak_ptr<project> _project;

    track_presenter(std::shared_ptr<project> const &);
};
}  // namespace yas::ae

//
//  ae_track_presenter.h
//

#pragma once

#include <audio_editor_core/ae_file_loader_types.h>
#include <audio_editor_core/ae_file_track_types.h>
#include <audio_editor_core/ae_marker_element.h>
#include <audio_editor_core/ae_marker_pool.h>
#include <audio_editor_core/ae_module_element.h>
#include <observing/yas_observing_umbrella.h>

namespace yas::ae {
class project;

struct track_presenter {
    [[nodiscard]] static std::shared_ptr<track_presenter> make_shared(std::string const &project_id);
    [[nodiscard]] static std::shared_ptr<track_presenter> make_shared(std::shared_ptr<project> const &);

    [[nodiscard]] std::vector<module_element> module_elements() const;
    [[nodiscard]] observing::syncable observe_modules(std::function<void(std::vector<module_element> const &)> &&);

    [[nodiscard]] double current_time() const;

   private:
    std::weak_ptr<project> _project;

    track_presenter(std::shared_ptr<project> const &);
};
}  // namespace yas::ae

//
//  ae_markers_presenter.h
//

#pragma once

#include <audio_editor_core/ae_marker_location.h>
#include <audio_editor_core/ae_markers_presenter_dependency.h>
#include <observing/yas_observing_umbrella.h>

namespace yas::ae {
struct markers_presenter {
    [[nodiscard]] static std::shared_ptr<markers_presenter> make_shared(std::string const &project_id);
    [[nodiscard]] static std::shared_ptr<markers_presenter> make_shared(
        std::shared_ptr<project_editor_for_markers_presenter> const &);

    [[nodiscard]] std::vector<marker_location> marker_locations() const;
    [[nodiscard]] observing::syncable observe_markers(std::function<void(std::vector<marker_location> const &)> &&);

   private:
    std::weak_ptr<project_editor_for_markers_presenter> _project_editor;

    markers_presenter(std::shared_ptr<project_editor_for_markers_presenter> const &);
};
}  // namespace yas::ae

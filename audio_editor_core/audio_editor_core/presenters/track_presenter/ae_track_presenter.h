//
//  ae_track_presenter.h
//

#pragma once

#include <audio_editor_core/ae_track_presenter_dependency.h>

namespace yas::ae {
struct track_presenter {
    [[nodiscard]] static std::shared_ptr<track_presenter> make_shared(std::string const &project_id);
    [[nodiscard]] static std::shared_ptr<track_presenter> make_shared(
        std::shared_ptr<project_editor_for_track_presenter> const &,
        std::shared_ptr<zooming_for_track_presenter> const &);

    [[nodiscard]] double scale() const;
    [[nodiscard]] observing::syncable observe_scale(std::function<void(double const &)> &&);

    [[nodiscard]] float current_position() const;

   private:
    std::weak_ptr<project_editor_for_track_presenter> _project_editor;
    std::weak_ptr<zooming_for_track_presenter> _zooming;

    track_presenter(std::shared_ptr<project_editor_for_track_presenter> const &,
                    std::shared_ptr<zooming_for_track_presenter> const &);

    track_presenter(track_presenter const &) = delete;
    track_presenter(track_presenter &&) = delete;
    track_presenter &operator=(track_presenter const &) = delete;
    track_presenter &operator=(track_presenter &&) = delete;
};
}  // namespace yas::ae

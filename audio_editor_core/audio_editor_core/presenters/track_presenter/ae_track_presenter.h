//
//  ae_track_presenter.h
//

#pragma once

#include <audio_editor_core/ae_file_info.h>
#include <observing/yas_observing_umbrella.h>

namespace yas::ae {
class player;
class zooming_pair;

struct track_presenter final {
    [[nodiscard]] static std::shared_ptr<track_presenter> make_shared(std::string const &project_id);

    [[nodiscard]] double horizontal_zooming_scale() const;
    [[nodiscard]] double vertical_zooming_scale() const;
    [[nodiscard]] observing::syncable observe_horizontal_zooming_scale(std::function<void(double const &)> &&);
    [[nodiscard]] observing::syncable observe_vertical_zooming_scale(std::function<void(double const &)> &&);

   private:
    std::weak_ptr<zooming_pair> _zooming_pair;

    track_presenter(std::shared_ptr<zooming_pair> const &);

    track_presenter(track_presenter const &) = delete;
    track_presenter(track_presenter &&) = delete;
    track_presenter &operator=(track_presenter const &) = delete;
    track_presenter &operator=(track_presenter &&) = delete;
};
}  // namespace yas::ae

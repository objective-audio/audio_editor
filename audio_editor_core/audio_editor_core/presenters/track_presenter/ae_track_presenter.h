//
//  ae_track_presenter.h
//

#pragma once

#include <audio_editor_core/ae_file_info.h>
#include <audio_editor_core/ae_track_presenter_dependency.h>
#include <observing/yas_observing_umbrella.h>

namespace yas::ae {
class player;
class zooming;

struct track_presenter final {
    [[nodiscard]] static std::shared_ptr<track_presenter> make_shared(std::string const &project_id);

    [[nodiscard]] double horizontal_zooming_scale() const;
    [[nodiscard]] double vertical_zooming_scale() const;
    [[nodiscard]] observing::syncable observe_horizontal_zooming_scale(std::function<void(double const &)> &&);
    [[nodiscard]] observing::syncable observe_vertical_zooming_scale(std::function<void(double const &)> &&);

    [[nodiscard]] float current_position() const;

   private:
    file_info const _file_info;
    std::weak_ptr<player> _player;
    std::weak_ptr<zooming> _horizontal_zooming;
    std::weak_ptr<zooming> _vertical_zooming;

    track_presenter(file_info const &, std::shared_ptr<player> const &,
                    std::shared_ptr<zooming> const &horizontal_zooming,
                    std::shared_ptr<zooming> const &vertical_zooming);

    track_presenter(track_presenter const &) = delete;
    track_presenter(track_presenter &&) = delete;
    track_presenter &operator=(track_presenter const &) = delete;
    track_presenter &operator=(track_presenter &&) = delete;
};
}  // namespace yas::ae

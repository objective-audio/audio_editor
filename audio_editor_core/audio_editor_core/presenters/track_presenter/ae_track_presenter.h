//
//  ae_track_presenter.h
//

#pragma once

#include <audio_editor_core/ae_file_info.h>
#include <audio_editor_core/ae_window_lifetime_id.h>
#include <audio_editor_core/ae_zooming_types.h>
#include <observing/yas_observing_umbrella.h>

namespace yas::ae {
class player;
class zooming_pair;

struct track_presenter final {
    [[nodiscard]] static std::shared_ptr<track_presenter> make_shared(window_lifetime_id const &);

    track_presenter(std::shared_ptr<zooming_pair> const &);

    [[nodiscard]] ae::zooming_scale zooming_scale() const;
    [[nodiscard]] observing::syncable observe_zooming_scale(std::function<void(ae::zooming_scale const &)> &&);

   private:
    std::weak_ptr<zooming_pair> const _zooming_pair;

    track_presenter(track_presenter const &) = delete;
    track_presenter(track_presenter &&) = delete;
    track_presenter &operator=(track_presenter const &) = delete;
    track_presenter &operator=(track_presenter &&) = delete;
};
}  // namespace yas::ae

//
//  ae_zooming.h
//

#pragma once

#include <audio_editor_core/ae_pinch_gesture_controller_dependency.h>
#include <audio_editor_core/ae_track_presenter_dependency.h>
#include <observing/yas_observing_umbrella.h>

namespace yas::ae {
struct zooming final : zooming_for_pinch_gesture_controller {
    [[nodiscard]] static std::shared_ptr<zooming> make_shared();

    void begin() override;
    void set_magnification(double const) override;
    void end() override;

    [[nodiscard]] double scale() const;

    [[nodiscard]] observing::syncable observe_scale(std::function<void(double const &)> &&);

   private:
    observing::value::holder_ptr<double> const _scale;

    std::optional<double> _began_scale = std::nullopt;
    std::optional<double> _magnification = std::nullopt;

    zooming();
};
}  // namespace yas::ae

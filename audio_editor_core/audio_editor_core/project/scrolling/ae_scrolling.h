//
//  ae_scrolling.h
//

#pragma once

#include <audio_editor_core/ae_scrolling_types.h>
#include <observing/yas_observing_umbrella.h>

#include <memory>
#include <optional>

namespace yas::ae {
struct scrolling {
    [[nodiscard]] static std::shared_ptr<scrolling> make_shared();

    bool is_enabled() const;
    void set_enabled(bool const);

    void begin();
    void set_delta_time(double const);
    void end();

    [[nodiscard]] observing::endable observe(std::function<void(scrolling_event const &)> &&);

   private:
    observing::notifier_ptr<scrolling_event> const _notifier;

    bool _is_enabled = true;
    bool _began = false;

    scrolling();
};
}  // namespace yas::ae

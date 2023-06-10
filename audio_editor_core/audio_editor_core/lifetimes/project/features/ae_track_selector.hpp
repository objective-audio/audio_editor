//
//  ae_track_selector.hpp
//

#pragma once

#include <audio_editor_core/ae_common_types.h>
#include <audio_editor_core/ae_track_selector_dependencies.h>

namespace yas::ae {
struct track_selector final {
    track_selector(scrolling_for_track_selector *);

    [[nodiscard]] track_index_t current() const;
    [[nodiscard]] observing::syncable observe_current(std::function<void(track_index_t const &)> &&);

   private:
    scrolling_for_track_selector *scrolling;

    observing::value::holder_ptr<track_index_t> const _current;
    observing::canceller_pool _cancellers;

    track_selector(track_selector const &) = delete;
    track_selector(track_selector &&) = delete;
    track_selector &operator=(track_selector const &) = delete;
    track_selector &operator=(track_selector &&) = delete;
};
}  // namespace yas::ae

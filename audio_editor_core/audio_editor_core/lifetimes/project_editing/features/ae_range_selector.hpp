//
//  ae_range_selector.hpp
//

#pragma once

#include <audio_editor_core/ae_range_selector_dependency.h>
#include <observing/yas_observing_umbrella.h>

#include <audio_editor_core/ae_point_range.hpp>

namespace yas::ae {
struct range_selector final {
    range_selector(player_for_range_selector *, deselector_for_range_selector *);

    void begin(ui::point const &);
    void move(ui::point const &);
    void end();

    [[nodiscard]] std::optional<ui::region> region() const;
    [[nodiscard]] observing::syncable observe_region(std::function<void(std::optional<ui::region> const &)> &&) const;

   private:
    observing::value::holder_ptr<std::optional<point_range>> const _range;
    player_for_range_selector *_player;
    deselector_for_range_selector *_deselector;

    observing::canceller_pool _pool;

    range_selector(range_selector const &) = delete;
    range_selector(range_selector &&) = delete;
    range_selector &operator=(range_selector const &) = delete;
    range_selector &operator=(range_selector &&) = delete;
};
}  // namespace yas::ae

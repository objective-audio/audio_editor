//
//  player.h
//

#pragma once

#include <ae-core/global/value_types/common_types.h>
#include <ae-core/global/value_types/project_guid.h>
#include <ae-core/project/features/player_dependency.h>
#include <ae-core/project_editing/features/playing_toggler_dependency.h>
#include <ae-core/project_editing/features/range_selector_dependency.h>
#include <audio-engine/common/types.h>
#include <audio-processing/common/ptr.h>

#include <filesystem>

namespace yas::ae {
struct player final : player_for_playing_toggler, player_for_range_selector {
    player(std::filesystem::path const &root_path, project_guid const &project_id, scrolling_for_player *);
    player(std::shared_ptr<playing::coordinator> const &, project_guid const &, scrolling_for_player *);

    void begin_rendering();

    void set_timeline(std::shared_ptr<proc::timeline> const &, sample_rate_t const, audio::pcm_format const);
    void reset_timeline();

    void set_playing(bool const) override;
    [[nodiscard]] bool is_playing() const override;

    void seek(frame_index_t const);

    [[nodiscard]] bool is_scrolling() const;

    [[nodiscard]] frame_index_t current_frame() const;

    [[nodiscard]] observing::syncable observe_is_playing(std::function<void(bool const &)> &&) override;

   private:
    project_guid const _project_id;
    std::shared_ptr<playing::coordinator> const _coordinator;
    scrolling_for_player *const _scrolling;

    std::optional<frame_index_t> _reserved_frame = std::nullopt;
    std::optional<frame_index_t> _seeking_frame = std::nullopt;

    observing::canceller_pool _pool;

    player(player const &) = delete;
    player(player &&) = delete;
    player &operator=(player const &) = delete;
    player &operator=(player &&) = delete;

    void _update_reserved_frame_if_began(double const delta_time);
};
}  // namespace yas::ae

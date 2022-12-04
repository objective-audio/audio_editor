//
//  ae_player.h
//

#pragma once

#include <audio/yas_audio_types.h>
#include <audio_editor_core/ae_common_types.h>
#include <audio_editor_core/ae_player_dependency.h>
#include <audio_editor_core/ae_playing_toggler_dependency.h>
#include <audio_editor_core/ae_project_id.h>
#include <processing/yas_processing_ptr.h>

#include <filesystem>

namespace yas::ae {
struct player final : player_for_playing_toggler {
    player(std::filesystem::path const &root_path, project_id const &project_id, scrolling_for_player *);
    player(std::shared_ptr<playing::coordinator> const &, project_id const &, scrolling_for_player *);

    void begin_rendering();

    void set_timeline(std::shared_ptr<proc::timeline> const &, sample_rate_t const, audio::pcm_format const);
    void reset_timeline();

    void set_playing(bool const) override;
    [[nodiscard]] bool is_playing() const override;

    void seek(frame_index_t const);

    [[nodiscard]] bool is_scrolling() const;

    [[nodiscard]] frame_index_t current_frame() const;

    [[nodiscard]] observing::syncable observe_is_playing(std::function<void(bool const &)> &&);

   private:
    project_id const _project_id;
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

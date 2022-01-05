//
//  ae_player.h
//

#pragma once

#include <audio_editor_core/ae_common_types.h>
#include <audio_editor_core/ae_player_dependency.h>
#include <audio_editor_core/ae_project_dependency.h>
#include <audio_editor_core/ae_project_editor_dependency.h>

#include <memory>

namespace yas::ae {
struct player final : player_for_project, player_for_project_editor {
    void begin_rendering() override;

    void set_timeline(std::shared_ptr<proc::timeline> const &, playing::sample_rate_t const,
                      audio::pcm_format const) override;
    void reset_timeline() override;
    void set_playing(bool const) override;
    [[nodiscard]] bool is_playing() const override;
    void seek(frame_index_t const) override;
    [[nodiscard]] bool is_scrolling() const override;
    [[nodiscard]] frame_index_t current_frame() const override;

    [[nodiscard]] observing::syncable observe_is_playing(std::function<void(bool const &)> &&) override;

    [[nodiscard]] static std::shared_ptr<player> make_shared(url const &root_url, std::string const &identifier,
                                                             std::shared_ptr<scrolling_for_player> const &);
    [[nodiscard]] static std::shared_ptr<player> make_shared(std::shared_ptr<playing::coordinator> const &,
                                                             std::string const &identifier,
                                                             std::shared_ptr<scrolling_for_player> const &);

   private:
    std::string const _identifier;
    std::shared_ptr<playing::coordinator> const _coordinator;
    std::shared_ptr<scrolling_for_player> const _scrolling;

    std::optional<frame_index_t> _reserved_frame = std::nullopt;
    std::optional<frame_index_t> _seeking_frame = std::nullopt;

    observing::canceller_pool _pool;

    player(std::shared_ptr<playing::coordinator> const &, std::string const &identifier,
           std::shared_ptr<scrolling_for_player> const &);

    player(player const &) = delete;
    player(player &&) = delete;
    player &operator=(player const &) = delete;
    player &operator=(player &&) = delete;

    void _update_reserved_frame_if_began(double const delta_time);
};
}  // namespace yas::ae

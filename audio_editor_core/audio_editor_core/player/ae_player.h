//
//  ae_player.h
//

#pragma once

#include <audio_editor_core/ae_player_dependency.h>
#include <audio_editor_core/ae_player_types.h>
#include <audio_editor_core/ae_project_dependency.h>
#include <audio_editor_core/ae_timeline_editor_dependency.h>

#include <memory>

namespace yas::ae {
struct player final : project_player_interface, timeline_player_interface {
    void set_timeline(std::shared_ptr<proc::timeline> const &) override;
    void reset_timeline() override;
    void set_playing(bool const) override;
    bool is_playing() const override;
    void seek(frame_index_t const) override;
    frame_index_t current_frame() const override;

    [[nodiscard]] observing::syncable observe_is_playing(std::function<void(bool const &)> &&) override;

    static std::shared_ptr<player> make_shared(url const &root_url, std::string const &identifier);
    static std::shared_ptr<player> make_shared(std::shared_ptr<playing::coordinator> const &,
                                               std::string const &identifier);

   private:
    std::string const _identifier;
    std::shared_ptr<playing::coordinator> const _coordinator;

    player(std::shared_ptr<playing::coordinator> const &, std::string const &identifier);

    player(player const &) = delete;
    player(player &&) = delete;
    player &operator=(player const &) = delete;
    player &operator=(player &&) = delete;
};
}  // namespace yas::ae

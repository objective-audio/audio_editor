//
//  ae_project_editor_dependency.h
//

#pragma once

#include <audio/yas_audio_umbrella.h>
#include <audio_editor_core/ae_file_loader_types.h>
#include <audio_editor_core/ae_file_module.h>
#include <audio_editor_core/ae_file_track_types.h>
#include <playing/yas_playing_umbrella.h>

#include <memory>

namespace yas::proc {
class timeline;
}

namespace yas::ae {
struct project_editor_file_loader_interface {
    virtual ~project_editor_file_loader_interface() = default;

    virtual std::optional<file_info> load_file_info(url const &) const = 0;
};

struct project_editor_player_interface {
    virtual ~project_editor_player_interface() = default;

    virtual void set_timeline(std::shared_ptr<proc::timeline> const &, playing::sample_rate_t const,
                              audio::pcm_format const) = 0;
    virtual void reset_timeline() = 0;
};

struct project_editor_file_track_interface {
    virtual ~project_editor_file_track_interface() = default;

    virtual void replace_modules(std::vector<file_module> &&) = 0;
    virtual void insert_module(file_module const &) = 0;
    virtual void erase_module(file_module const &) = 0;

    virtual std::optional<file_module> module(proc::frame_index_t const) const = 0;
    virtual std::optional<file_module> previous_module(proc::frame_index_t const) const = 0;
    virtual std::optional<file_module> next_module(proc::frame_index_t const) const = 0;
    virtual std::optional<file_module> splittable_module(proc::frame_index_t const) const = 0;
    virtual void split(proc::frame_index_t const) = 0;
    virtual void drop_head(proc::frame_index_t const) = 0;
    virtual void drop_tail(proc::frame_index_t const) = 0;
    virtual void drop_head_and_offset(proc::frame_index_t const) = 0;
    virtual void drop_tail_and_offset(proc::frame_index_t const) = 0;
    virtual void overwrite_module(file_module const &) = 0;
    virtual void move_modules(std::set<proc::time::range> const &, proc::frame_index_t const) = 0;

    [[nodiscard]] virtual observing::syncable observe_event(std::function<void(file_track_event const &)> &&) = 0;
};
}  // namespace yas::ae

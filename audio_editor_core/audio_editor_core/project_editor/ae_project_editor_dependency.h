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
struct file_loader_for_project_editor {
    virtual ~file_loader_for_project_editor() = default;

    virtual std::optional<file_info> load_file_info(url const &) const = 0;
};

struct player_for_project_editor {
    virtual ~player_for_project_editor() = default;

    virtual void set_timeline(std::shared_ptr<proc::timeline> const &, playing::sample_rate_t const,
                              audio::pcm_format const) = 0;
    virtual void reset_timeline() = 0;
};

struct file_track_for_project_editor {
    virtual ~file_track_for_project_editor() = default;

    [[nodiscard]] virtual file_track_module_map_t const &modules() const = 0;

    virtual void replace_modules_and_notify(std::vector<file_module> &&) = 0;
    virtual void insert_module_and_notify(file_module const &) = 0;
    virtual void erase_module_and_notify(file_module const &) = 0;

    virtual std::optional<file_module> module_at(proc::frame_index_t const) const = 0;
    virtual std::optional<file_module> previous_module_at(proc::frame_index_t const) const = 0;
    virtual std::optional<file_module> next_module_at(proc::frame_index_t const) const = 0;
    virtual std::optional<file_module> splittable_module_at(proc::frame_index_t const) const = 0;
    virtual std::optional<proc::frame_index_t> next_edge(proc::frame_index_t const) const = 0;
    virtual std::optional<proc::frame_index_t> previous_edge(proc::frame_index_t const) const = 0;
    virtual void split_at(proc::frame_index_t const) = 0;
    virtual void erase_at(proc::frame_index_t const) = 0;
    virtual void erase_and_offset_at(proc::frame_index_t const) = 0;
    virtual void drop_head_at(proc::frame_index_t const) = 0;
    virtual void drop_tail_at(proc::frame_index_t const) = 0;
    virtual void drop_head_and_offset_at(proc::frame_index_t const) = 0;
    virtual void drop_tail_and_offset_at(proc::frame_index_t const) = 0;
    virtual void overwrite_module(file_module const &) = 0;
    virtual void move_modules(std::set<proc::time::range> const &, proc::frame_index_t const) = 0;

    [[nodiscard]] virtual observing::syncable observe_event(std::function<void(file_track_event const &)> &&) = 0;
};

struct marker_pool_for_project_editor {
    virtual ~marker_pool_for_project_editor() = default;
};
}  // namespace yas::ae

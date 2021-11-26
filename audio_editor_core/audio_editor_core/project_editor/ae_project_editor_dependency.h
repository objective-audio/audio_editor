//
//  ae_project_editor_dependency.h
//

#pragma once

#include <audio_editor_core/ae_db_pasting_subject.h>
#include <audio_editor_core/ae_db_types.h>
#include <audio_editor_core/ae_exporter_types.h>
#include <audio_editor_core/ae_file_loader_types.h>
#include <audio_editor_core/ae_file_module.h>
#include <audio_editor_core/ae_file_track_types.h>
#include <audio_editor_core/ae_marker_pool_types.h>
#include <audio_editor_core/ae_pasteboard_types.h>

namespace yas::proc {
class timeline;
}

namespace yas::ae {
struct file_loader_for_project_editor {
    virtual ~file_loader_for_project_editor() = default;

    [[nodiscard]] virtual std::optional<file_info> load_file_info(url const &) const = 0;
};

struct player_for_project_editor {
    virtual ~player_for_project_editor() = default;

    virtual void set_timeline(std::shared_ptr<proc::timeline> const &, playing::sample_rate_t const,
                              audio::pcm_format const) = 0;
    virtual void reset_timeline() = 0;

    virtual void set_playing(bool const) = 0;
    [[nodiscard]] virtual bool is_playing() const = 0;

    virtual void seek(proc::frame_index_t const) = 0;
    [[nodiscard]] virtual proc::frame_index_t current_frame() const = 0;
    virtual bool is_scrolling() const = 0;

    [[nodiscard]] virtual observing::syncable observe_is_playing(std::function<void(bool const &)> &&) = 0;
};

struct file_track_for_project_editor {
    virtual ~file_track_for_project_editor() = default;

    [[nodiscard]] virtual file_track_module_map_t const &modules() const = 0;

    virtual void revert_modules_and_notify(std::vector<file_module> &&) = 0;
    virtual void insert_module_and_notify(file_module const &) = 0;
    virtual void erase_module_and_notify(file_module const &) = 0;

    [[nodiscard]] virtual std::optional<file_module> module_at(proc::frame_index_t const) const = 0;
    [[nodiscard]] virtual std::optional<file_module> previous_module_at(proc::frame_index_t const) const = 0;
    [[nodiscard]] virtual std::optional<file_module> next_module_at(proc::frame_index_t const) const = 0;
    [[nodiscard]] virtual std::optional<file_module> splittable_module_at(proc::frame_index_t const) const = 0;
    [[nodiscard]] virtual std::optional<proc::frame_index_t> next_edge(proc::frame_index_t const) const = 0;
    [[nodiscard]] virtual std::optional<proc::frame_index_t> previous_edge(proc::frame_index_t const) const = 0;
    virtual void split_at(proc::frame_index_t const) = 0;
    virtual void erase_at(proc::frame_index_t const) = 0;
    virtual void erase_and_offset_at(proc::frame_index_t const) = 0;
    virtual void drop_head_at(proc::frame_index_t const) = 0;
    virtual void drop_tail_at(proc::frame_index_t const) = 0;
    virtual void drop_head_and_offset_at(proc::frame_index_t const) = 0;
    virtual void drop_tail_and_offset_at(proc::frame_index_t const) = 0;
    virtual void overwrite_module(file_module const &) = 0;
    virtual void move_modules(std::set<proc::time::range> const &, proc::frame_index_t const) = 0;
    virtual void split_and_insert_module_and_offset(file_module const &) = 0;

    [[nodiscard]] virtual observing::syncable observe_event(std::function<void(file_track_event const &)> &&) = 0;
};

struct marker_pool_for_project_editor {
    virtual ~marker_pool_for_project_editor() = default;

    virtual void revert_markers(std::vector<marker> &&) = 0;
    virtual void insert_marker(marker const &) = 0;
    virtual void erase_at(proc::frame_index_t const) = 0;
    virtual void erase_marker(marker const &) = 0;

    [[nodiscard]] virtual std::map<proc::frame_index_t, marker> const &markers() const = 0;
    [[nodiscard]] virtual std::optional<marker> marker_at(std::size_t const) const = 0;

    [[nodiscard]] virtual std::optional<proc::frame_index_t> next_edge(proc::frame_index_t const) const = 0;
    [[nodiscard]] virtual std::optional<proc::frame_index_t> previous_edge(proc::frame_index_t const) const = 0;

    [[nodiscard]] virtual observing::syncable observe_event(std::function<void(marker_pool_event const &)> &&) = 0;
};

struct database_for_project_editor {
    virtual ~database_for_project_editor() = default;

    [[nodiscard]] virtual db_modules_map const &modules() const = 0;
    [[nodiscard]] virtual db_markers_map const &markers() const = 0;
    [[nodiscard]] virtual std::string const pasting_data() const = 0;

    virtual void add_module(file_module const &) = 0;
    virtual void remove_module(proc::time::range const &) = 0;
    virtual void set_pasting_data(std::string const &) = 0;
    virtual void add_marker(marker const &) = 0;
    virtual void remove_marker(proc::frame_index_t const &) = 0;

    virtual void suspend_saving(std::function<void(void)> &&) = 0;

    [[nodiscard]] virtual bool can_undo() const = 0;
    virtual void undo() = 0;
    [[nodiscard]] virtual bool can_redo() const = 0;
    virtual void redo() = 0;

    [[nodiscard]] virtual observing::endable observe_reverted(std::function<void(void)> &&) = 0;
};

struct exporter_for_project_editor {
    virtual ~exporter_for_project_editor() = default;

    virtual void begin(url const &export_url, std::shared_ptr<proc::timeline> const &, exporting_format const &) = 0;
    [[nodiscard]] virtual bool is_exporting() const = 0;
    [[nodiscard]] virtual observing::syncable observe_is_exporting(std::function<void(bool const &)> &&) = 0;
};

struct pasteboard_for_project_editor {
    virtual ~pasteboard_for_project_editor() = default;

    [[nodiscard]] virtual std::optional<pasting_file_module> file_module() const = 0;
    virtual void set_file_module(pasting_file_module const &) = 0;

    [[nodiscard]] virtual std::string const &data() const = 0;
    virtual void revert_data(std::string const &) = 0;

    [[nodiscard]] virtual observing::syncable observe_event(std::function<void(pasteboard_event const &)> &&) = 0;
};
}  // namespace yas::ae

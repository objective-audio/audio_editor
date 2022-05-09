//
//  ae_project_editor_dependency.h
//

#pragma once

#include <audio_editor_core/ae_db_pasting_subject.h>
#include <audio_editor_core/ae_db_types.h>
#include <audio_editor_core/ae_edge_holder_types.h>
#include <audio_editor_core/ae_exporter_types.h>
#include <audio_editor_core/ae_file_info.h>
#include <audio_editor_core/ae_file_module.h>
#include <audio_editor_core/ae_file_track_types.h>
#include <audio_editor_core/ae_marker_pool_types.h>
#include <audio_editor_core/ae_nudge_settings_dependency.h>
#include <audio_editor_core/ae_pasteboard_types.h>
#include <audio_editor_core/ae_time_editor_types.h>
#include <audio_editor_core/ae_timing_types.h>

namespace yas::proc {
class timeline;
}

namespace yas::ae {
struct player_for_project_editor {
    virtual ~player_for_project_editor() = default;

    virtual void begin_rendering() = 0;
    virtual void set_timeline(std::shared_ptr<proc::timeline> const &, playing::sample_rate_t const,
                              audio::pcm_format const) = 0;
    virtual void reset_timeline() = 0;

    virtual void set_playing(bool const) = 0;
    [[nodiscard]] virtual bool is_playing() const = 0;

    virtual void seek(frame_index_t const) = 0;
    [[nodiscard]] virtual frame_index_t current_frame() const = 0;
    virtual bool is_scrolling() const = 0;

    [[nodiscard]] virtual observing::syncable observe_is_playing(std::function<void(bool const &)> &&) = 0;
};

struct file_track_for_project_editor {
    virtual ~file_track_for_project_editor() = default;

    [[nodiscard]] virtual file_track_module_map_t const &modules() const = 0;

    virtual void revert_modules_and_notify(std::vector<file_module> &&) = 0;
    virtual void insert_module_and_notify(file_module const &) = 0;
    virtual void erase_module_and_notify(file_module const &) = 0;

    [[nodiscard]] virtual std::optional<file_module> module_at(frame_index_t const) const = 0;
    [[nodiscard]] virtual std::optional<file_module> previous_module_at(frame_index_t const) const = 0;
    [[nodiscard]] virtual std::optional<file_module> next_module_at(frame_index_t const) const = 0;
    [[nodiscard]] virtual std::optional<file_module> splittable_module_at(frame_index_t const) const = 0;
    [[nodiscard]] virtual std::optional<file_module> first_module() const = 0;
    [[nodiscard]] virtual std::optional<file_module> last_module() const = 0;
    virtual void split_at(frame_index_t const) = 0;
    virtual void erase_at(frame_index_t const) = 0;
    virtual void erase_and_offset_at(frame_index_t const) = 0;
    virtual void drop_head_at(frame_index_t const) = 0;
    virtual void drop_tail_at(frame_index_t const) = 0;
    virtual void drop_head_and_offset_at(frame_index_t const) = 0;
    virtual void drop_tail_and_offset_at(frame_index_t const) = 0;
    virtual void overwrite_module(file_module const &) = 0;
    virtual void move_modules(std::set<time::range> const &, frame_index_t const) = 0;
    virtual void split_and_insert_module_and_offset(file_module const &) = 0;

    [[nodiscard]] virtual observing::syncable observe_event(std::function<void(file_track_event const &)> &&) = 0;
};

struct marker_pool_for_project_editor {
    virtual ~marker_pool_for_project_editor() = default;

    virtual void revert_markers(std::vector<marker> &&) = 0;
    virtual void insert_marker(marker const &) = 0;
    virtual void erase_at(frame_index_t const) = 0;
    virtual void erase_marker(marker const &) = 0;
    virtual void erase_range(time::range const) = 0;

    virtual void move_at(frame_index_t const frame, frame_index_t const new_frame) = 0;
    virtual void move_offset_from(frame_index_t const from, frame_index_t const offset) = 0;

    [[nodiscard]] virtual std::map<frame_index_t, marker> const &markers() const = 0;
    [[nodiscard]] virtual std::optional<marker> marker_at(std::size_t const) const = 0;

    [[nodiscard]] virtual observing::syncable observe_event(std::function<void(marker_pool_event const &)> &&) = 0;
};

struct edge_holder_for_project_editor {
    virtual ~edge_holder_for_project_editor() = default;

    [[nodiscard]] virtual ae::edge const &edge() const = 0;
    virtual void set_edge(ae::edge const &) = 0;
    virtual void set_begin_frame(frame_index_t const) = 0;
    virtual void set_end_frame(frame_index_t const) = 0;
    virtual void revert_edge(ae::edge const &) = 0;

    [[nodiscard]] virtual observing::syncable observe_event(std::function<void(edge_holder_event const &)> &&) = 0;
};

struct database_for_project_editor {
    virtual ~database_for_project_editor() = default;

    [[nodiscard]] virtual db_modules_map const &modules() const = 0;
    [[nodiscard]] virtual db_markers_map const &markers() const = 0;
    [[nodiscard]] virtual std::optional<db_edge> const &edge() const = 0;
    [[nodiscard]] virtual std::string const pasting_data() const = 0;

    virtual void add_module(file_module const &) = 0;
    virtual void remove_module(time::range const &) = 0;
    virtual void update_module_detail(file_module const &) = 0;
    virtual void set_pasting_data(std::string const &) = 0;
    virtual void add_marker(marker const &) = 0;
    virtual void remove_marker(frame_index_t const &) = 0;
    virtual void set_edge(ae::edge const &) = 0;

    virtual void suspend_saving(std::function<void(void)> &&) = 0;

    [[nodiscard]] virtual bool can_undo() const = 0;
    virtual void undo() = 0;
    [[nodiscard]] virtual bool can_redo() const = 0;
    virtual void redo() = 0;

    [[nodiscard]] virtual observing::endable observe_reverted(std::function<void(void)> &&) = 0;
};

struct exporter_for_project_editor {
    virtual ~exporter_for_project_editor() = default;

    virtual void begin(url const &export_url, std::shared_ptr<proc::timeline> const &, exporting_format const &,
                       time::range const &) = 0;
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

struct nudge_settings_for_project_editor {
    virtual ~nudge_settings_for_project_editor() = default;

    virtual void rotate_next_unit() = 0;
    virtual void rotate_previous_unit() = 0;
    [[nodiscard]] virtual std::size_t unit_index() const = 0;
    [[nodiscard]] virtual observing::syncable observe_unit_index(std::function<void(std::size_t const &)> &&) = 0;

    [[nodiscard]] virtual std::optional<frame_index_t> next_frame(frame_index_t const current_frame,
                                                                  uint32_t const offset_count) const = 0;
    [[nodiscard]] virtual std::optional<frame_index_t> previous_frame(frame_index_t const current_frame,
                                                                      uint32_t const offset_count) const = 0;
};

struct timing_for_project_editor : timing_for_nudge_settings {
    virtual ~timing_for_project_editor() = default;

    virtual void set_fraction_kind(timing_fraction_kind const) = 0;
    [[nodiscard]] virtual timing_fraction_kind fraction_kind() const = 0;
    [[nodiscard]] virtual observing::syncable observe_fraction_kind(
        std::function<void(timing_fraction_kind const &)> &&) = 0;

    [[nodiscard]] virtual timing_components components(frame_index_t const) const = 0;
};
}  // namespace yas::ae

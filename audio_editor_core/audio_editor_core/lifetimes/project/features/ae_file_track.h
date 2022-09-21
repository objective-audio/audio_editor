//
//  ae_file_track.h
//

#pragma once

#include <audio_editor_core/ae_common_types.h>
#include <audio_editor_core/ae_file_track_dependencies.h>
#include <audio_editor_core/ae_file_track_types.h>
#include <audio_editor_core/ae_jumper_dependency.h>
#include <observing/yas_observing_umbrella.h>

#include <set>

namespace yas::ae {
struct file_track final : jumpable_on_jumper {
    [[nodiscard]] static std::shared_ptr<file_track> make_shared(database_for_file_track *);

    file_track(database_for_file_track *);

    [[nodiscard]] file_track_module_map_t const &modules() const;

    void revert_modules_and_notify(std::vector<file_module_object> &&);
    std::optional<object_id> insert_module_and_notify(file_module const &);
    void erase_module_and_notify(time::range const &ranga);
    void set_module_name_and_notify(time::range const &, std::string const &);

    [[nodiscard]] std::optional<time::range> total_range() const;
    [[nodiscard]] std::optional<file_module_object> module_at(frame_index_t const) const;
    [[nodiscard]] std::optional<file_module_object> previous_module_at(frame_index_t const) const;
    [[nodiscard]] std::optional<file_module_object> next_module_at(frame_index_t const) const;
    [[nodiscard]] std::optional<file_module_object> splittable_module_at(frame_index_t const) const;
    [[nodiscard]] std::optional<file_module_object> first_module() const;
    [[nodiscard]] std::optional<file_module_object> last_module() const;
    [[nodiscard]] std::optional<frame_index_t> next_jumpable_frame(frame_index_t const) const override;
    [[nodiscard]] std::optional<frame_index_t> previous_jumpable_frame(frame_index_t const) const override;

    void split_at(frame_index_t const);
    void erase_at(frame_index_t const);
    void erase_and_offset_at(frame_index_t const);
    void drop_head_at(frame_index_t const);
    void drop_tail_at(frame_index_t const);
    void drop_head_and_offset_at(frame_index_t const);
    void drop_tail_and_offset_at(frame_index_t const);
    void overwrite_module(file_module const &);
    void move_modules(std::set<time::range> const &, frame_index_t const offset);
    void split_and_insert_module_and_offset(file_module const &);

    [[nodiscard]] observing::syncable observe_event(std::function<void(file_track_event const &)> &&);

   private:
    database_for_file_track *_database;

    file_track_module_map_t _modules;

    observing::fetcher_ptr<file_track_event> _event_fetcher;

    file_track(file_track const &) = delete;
    file_track(file_track &&) = delete;
    file_track &operator=(file_track const &) = delete;
    file_track &operator=(file_track &&) = delete;

    void _move_modules_after(frame_index_t const frame, frame_index_t const offset);
};
}  // namespace yas::ae

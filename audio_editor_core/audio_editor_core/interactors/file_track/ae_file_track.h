//
//  ae_file_track.h
//

#pragma once

#include <audio_editor_core/ae_file_track_types.h>
#include <audio_editor_core/ae_jumper_dependency.h>
#include <audio_editor_core/ae_project_editor_dependency.h>
#include <observing/yas_observing_umbrella.h>

namespace yas::ae {
struct file_track final : file_track_for_project_editor, jumpable_on_jumper {
    [[nodiscard]] static std::shared_ptr<file_track> make_shared();

    file_track();

    [[nodiscard]] file_track_module_map_t const &modules() const override;

    void revert_modules_and_notify(std::vector<file_module> &&) override;
    void insert_module_and_notify(file_module const &) override;
    void erase_module_and_notify(file_module const &) override;
    void set_module_name_and_notify(time::range const &, std::string const &);

    std::optional<file_module> module_at(frame_index_t const) const override;
    std::optional<file_module> previous_module_at(frame_index_t const) const override;
    std::optional<file_module> next_module_at(frame_index_t const) const override;
    std::optional<file_module> splittable_module_at(frame_index_t const) const override;
    std::optional<file_module> first_module() const override;
    std::optional<file_module> last_module() const override;
    [[nodiscard]] std::optional<frame_index_t> next_jumpable_frame(frame_index_t const) const override;
    [[nodiscard]] std::optional<frame_index_t> previous_jumpable_frame(frame_index_t const) const override;

    void split_at(frame_index_t const) override;
    void erase_at(frame_index_t const) override;
    void erase_and_offset_at(frame_index_t const) override;
    void drop_head_at(frame_index_t const) override;
    void drop_tail_at(frame_index_t const) override;
    void drop_head_and_offset_at(frame_index_t const) override;
    void drop_tail_and_offset_at(frame_index_t const) override;
    void overwrite_module(file_module const &) override;
    void move_modules(std::set<time::range> const &, frame_index_t const offset) override;
    void split_and_insert_module_and_offset(file_module const &) override;

    [[nodiscard]] observing::syncable observe_event(std::function<void(file_track_event const &)> &&) override;

   private:
    file_track_module_map_t _modules;

    observing::fetcher_ptr<file_track_event> _event_fetcher;

    file_track(file_track const &) = delete;
    file_track(file_track &&) = delete;
    file_track &operator=(file_track const &) = delete;
    file_track &operator=(file_track &&) = delete;

    void _move_modules_after(frame_index_t const frame, frame_index_t const offset);
};
}  // namespace yas::ae

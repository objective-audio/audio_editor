//
//  ae_project_editor.h
//

#pragma once

#include <audio_editor_core/ae_editing_root_presenter_dependency.h>
#include <audio_editor_core/ae_project_dependency.h>
#include <audio_editor_core/ae_project_editor_dependency.h>

namespace yas::ae {
struct project_editor final : project_editor_for_project {
    [[nodiscard]] ae::file_info const &file_info() const override;
    [[nodiscard]] std::shared_ptr<player_for_project_editor> const &player() const;
    [[nodiscard]] std::shared_ptr<file_track_for_project_editor> const &file_track() const;
    [[nodiscard]] std::shared_ptr<marker_pool_for_project_editor> const &marker_pool() const;
    [[nodiscard]] std::shared_ptr<database_for_project_editor> const &database() const;

    [[nodiscard]] proc::frame_index_t current_frame() const override;

    void set_playing(bool const) override;
    [[nodiscard]] bool is_playing() const override;

    [[nodiscard]] bool is_scrolling() const override;

    [[nodiscard]] bool can_nudge() const override;
    void nudge_previous() override;
    void nudge_next() override;

    [[nodiscard]] bool can_jump_to_previous_edge() const override;
    [[nodiscard]] bool can_jump_to_next_edge() const override;
    void jump_to_previous_edge() override;
    void jump_to_next_edge() override;

    [[nodiscard]] bool can_split() const override;
    void split() override;
    void drop_head_and_offset() override;
    void drop_tail_and_offset() override;

    [[nodiscard]] bool can_erase() const override;
    void erase_and_offset() override;

    [[nodiscard]] bool can_insert_marker() const override;
    void insert_marker() override;

    [[nodiscard]] bool can_return_to_zero() const override;
    void return_to_zero() override;

    [[nodiscard]] bool can_go_to_marker(std::size_t const) const override;
    void go_to_marker(std::size_t const) override;

    [[nodiscard]] bool can_undo() const override;
    void undo() override;

    [[nodiscard]] bool can_redo() const override;
    void redo() override;

    [[nodiscard]] std::optional<proc::frame_index_t> previous_edge() const override;
    [[nodiscard]] std::optional<proc::frame_index_t> next_edge() const override;

    [[nodiscard]] std::map<proc::frame_index_t, marker> const &markers() const override;
    [[nodiscard]] file_track_module_map_t const &modules() const override;

    [[nodiscard]] observing::syncable observe_is_playing(std::function<void(bool const &)> &&) override;
    [[nodiscard]] observing::syncable observe_file_track_event(
        std::function<void(file_track_event const &)> &&) override;
    [[nodiscard]] observing::syncable observe_marker_pool_event(
        std::function<void(marker_pool_event const &)> &&) override;

    [[nodiscard]] static std::shared_ptr<project_editor> make_shared(
        url const &editing_file_url, url const &db_url, ae::file_info const &,
        std::shared_ptr<player_for_project_editor> const &);
    [[nodiscard]] static std::shared_ptr<project_editor> make_shared(
        url const &editing_file_url, ae::file_info const &, std::shared_ptr<player_for_project_editor> const &,
        std::shared_ptr<file_track_for_project_editor> const &, std::shared_ptr<marker_pool_for_project_editor> const &,
        std::shared_ptr<database_for_project_editor> const &);

   private:
    url const _editing_file_url;
    ae::file_info const _file_info;
    std::shared_ptr<player_for_project_editor> const _player;
    std::shared_ptr<file_track_for_project_editor> const _file_track;
    std::shared_ptr<marker_pool_for_project_editor> const _marker_pool;
    std::shared_ptr<database_for_project_editor> const _database;

    proc::timeline_ptr const _timeline;
    proc::track_ptr _track;
    observing::canceller_pool _pool;

    project_editor(url const &editing_file_url, ae::file_info const &,
                   std::shared_ptr<player_for_project_editor> const &,
                   std::shared_ptr<file_track_for_project_editor> const &,
                   std::shared_ptr<marker_pool_for_project_editor> const &,
                   std::shared_ptr<database_for_project_editor> const &);

    project_editor(project_editor const &) = delete;
    project_editor(project_editor &&) = delete;
    project_editor &operator=(project_editor const &) = delete;
    project_editor &operator=(project_editor &&) = delete;
};
}  // namespace yas::ae

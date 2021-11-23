//
//  ae_project_editor.h
//

#pragma once

#include <audio_editor_core/ae_action_controller.h>
#include <audio_editor_core/ae_dialog_presenter.h>
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

    void set_playing(bool const);
    [[nodiscard]] bool is_playing() const override;

    [[nodiscard]] bool is_scrolling() const override;

    [[nodiscard]] bool can_nudge() const override;
    void nudge_previous();
    void nudge_next();

    [[nodiscard]] bool can_jump_to_previous_edge() const override;
    [[nodiscard]] bool can_jump_to_next_edge() const override;
    void jump_to_previous_edge();
    void jump_to_next_edge();

    [[nodiscard]] bool can_split() const override;
    void split();
    void drop_head_and_offset();
    void drop_tail_and_offset();

    [[nodiscard]] bool can_erase() const override;
    void erase_and_offset();

    [[nodiscard]] bool can_insert_marker() const override;
    void insert_marker();

    [[nodiscard]] bool can_return_to_zero() const override;
    void return_to_zero();

    [[nodiscard]] bool can_go_to_marker(std::size_t const) const override;
    void go_to_marker(std::size_t const);

    [[nodiscard]] bool can_undo() const override;
    void undo();

    [[nodiscard]] bool can_redo() const override;
    void redo();

    [[nodiscard]] bool can_select_file_for_export() const override;
    void select_file_for_export();
    [[nodiscard]] bool can_export_to_file() const;
    void export_to_file(url const &);

    [[nodiscard]] bool can_cut() const override;
    void cut();
    [[nodiscard]] bool can_copy() const override;
    void copy();
    [[nodiscard]] bool can_paste() const override;
    void paste();

    [[nodiscard]] std::map<proc::frame_index_t, marker> const &markers() const override;
    [[nodiscard]] file_track_module_map_t const &modules() const override;

    [[nodiscard]] observing::syncable observe_is_playing(std::function<void(bool const &)> &&) override;
    [[nodiscard]] observing::syncable observe_file_track_event(
        std::function<void(file_track_event const &)> &&) override;
    [[nodiscard]] observing::syncable observe_marker_pool_event(
        std::function<void(marker_pool_event const &)> &&) override;

    [[nodiscard]] static std::shared_ptr<project_editor> make_shared(url const &editing_file_url, url const &db_url,
                                                                     ae::file_info const &,
                                                                     std::shared_ptr<player_for_project_editor> const &,
                                                                     std::shared_ptr<action_controller> const &,
                                                                     std::shared_ptr<dialog_presenter> const &);
    [[nodiscard]] static std::shared_ptr<project_editor> make_shared(
        url const &editing_file_url, ae::file_info const &, std::shared_ptr<player_for_project_editor> const &,
        std::shared_ptr<file_track_for_project_editor> const &, std::shared_ptr<marker_pool_for_project_editor> const &,
        std::shared_ptr<pasteboard_for_project_editor> const &, std::shared_ptr<database_for_project_editor> const &,
        std::shared_ptr<exporter_for_project_editor> const &, std::shared_ptr<action_controller> const &,
        std::shared_ptr<dialog_presenter> const &);

   private:
    url const _editing_file_url;
    ae::file_info const _file_info;
    std::shared_ptr<player_for_project_editor> const _player;
    std::shared_ptr<file_track_for_project_editor> const _file_track;
    std::shared_ptr<marker_pool_for_project_editor> const _marker_pool;
    std::shared_ptr<pasteboard_for_project_editor> const _pasteboard;
    std::shared_ptr<database_for_project_editor> const _database;
    std::shared_ptr<exporter_for_project_editor> const _exporter;
    std::shared_ptr<action_controller> const _action_controller;
    std::shared_ptr<dialog_presenter> const _dialog_presenter;

    proc::timeline_ptr const _timeline;
    proc::track_ptr _track;
    observing::canceller_pool _pool;

    project_editor(url const &editing_file_url, ae::file_info const &,
                   std::shared_ptr<player_for_project_editor> const &,
                   std::shared_ptr<file_track_for_project_editor> const &,
                   std::shared_ptr<marker_pool_for_project_editor> const &,
                   std::shared_ptr<pasteboard_for_project_editor> const &,
                   std::shared_ptr<database_for_project_editor> const &,
                   std::shared_ptr<exporter_for_project_editor> const &, std::shared_ptr<action_controller> const &,
                   std::shared_ptr<dialog_presenter> const &);

    project_editor(project_editor const &) = delete;
    project_editor(project_editor &&) = delete;
    project_editor &operator=(project_editor const &) = delete;
    project_editor &operator=(project_editor &&) = delete;

    [[nodiscard]] std::optional<proc::frame_index_t> _previous_edge() const;
    [[nodiscard]] std::optional<proc::frame_index_t> _next_edge() const;
    [[nodiscard]] bool _can_editing() const;
};
}  // namespace yas::ae

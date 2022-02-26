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
struct project_editor final : project_editor_for_editing_root_presenter,
                              project_editor_for_markers_presenter,
                              project_editor_for_modules_presenter,
                              project_editor_for_track_presenter,
                              project_editor_for_time_presenter,
                              project_editor_for_edge_presenter {
    [[nodiscard]] static std::shared_ptr<project_editor> make_shared(
        std::string const &identifier, ae::file_info const &, std::shared_ptr<file_track_for_project_editor> const &,
        std::shared_ptr<marker_pool_for_project_editor> const &,
        std::shared_ptr<edge_editor_for_project_editor> const &, std::shared_ptr<pasteboard_for_project_editor> const &,
        std::shared_ptr<database_for_project_editor> const &, std::shared_ptr<exporter_for_project_editor> const &,
        std::shared_ptr<nudging_for_project_editor> const &, std::shared_ptr<timing_for_project_editor> const &,
        std::shared_ptr<time_editor_maker_for_project_editor> const &);

    [[nodiscard]] frame_index_t current_frame() const override;

    void set_playing(bool const);
    [[nodiscard]] bool is_playing() const override;

    [[nodiscard]] bool is_scrolling() const override;

    [[nodiscard]] bool can_nudge() const override;
    void nudge_previous(uint32_t const offset_count);
    void nudge_next(uint32_t const offset_count);
    [[nodiscard]] std::size_t nudging_unit_index() const override;
    void rotate_nudging_next_unit();
    void rotate_nudging_previous_unit();

    [[nodiscard]] ae::timing_fraction_kind timing_fraction_kind() const override;
    void rotate_timing_fraction();

    [[nodiscard]] bool can_jump_to_previous_edge() const override;
    [[nodiscard]] bool can_jump_to_next_edge() const override;
    [[nodiscard]] bool can_jump_to_beginnig() const override;
    [[nodiscard]] bool can_jump_to_end() const override;
    void jump_to_previous_edge();
    void jump_to_next_edge();
    void jump_to_beginning();
    void jump_to_end();

    [[nodiscard]] bool can_split() const override;
    void split();
    void drop_head();
    void drop_tail();
    void drop_head_and_offset();
    void drop_tail_and_offset();

    [[nodiscard]] bool can_erase() const override;
    void erase();
    void erase_and_offset();

    [[nodiscard]] ae::edge const &edge() const override;

    [[nodiscard]] bool can_insert_marker() const override;
    void insert_marker();

    [[nodiscard]] bool can_set_begin_edge() const;
    [[nodiscard]] bool can_set_end_edge() const;
    void set_begin_edge();
    void set_end_edge();

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
    void cut_and_offset();
    [[nodiscard]] bool can_copy() const override;
    void copy();
    [[nodiscard]] bool can_paste() const override;
    void paste_and_offset();

    [[nodiscard]] bool can_begin_time_editing() const override;
    [[nodiscard]] bool can_end_time_editing() const override;
    [[nodiscard]] bool can_input_time_number() const;
    void begin_time_editing();
    void finish_time_editing();
    void cancel_time_editing();
    void input_time_number(uint32_t const);
    void delete_time_number();
    void increment_time_number();
    void decrement_time_number();
    void move_to_previous_time_unit();
    void move_to_next_time_unit();
    void change_time_sign_to_plus();
    void change_time_sign_to_minus();
    void select_time_unit(std::size_t const);

    [[nodiscard]] std::map<frame_index_t, marker> const &markers() const override;
    [[nodiscard]] file_track_module_map_t const &modules() const override;

    [[nodiscard]] observing::syncable observe_is_playing(std::function<void(bool const &)> &&) override;
    [[nodiscard]] observing::syncable observe_file_track_event(
        std::function<void(file_track_event const &)> &&) override;
    [[nodiscard]] observing::syncable observe_marker_pool_event(
        std::function<void(marker_pool_event const &)> &&) override;
    [[nodiscard]] observing::syncable observe_edge_editor_event(
        std::function<void(edge_editor_event const &)> &&) override;
    [[nodiscard]] observing::syncable observe_nudging_unit_index(std::function<void(std::size_t const &)> &&) override;
    [[nodiscard]] observing::syncable observe_timing_fraction(
        std::function<void(ae::timing_fraction_kind const &)> &&) override;
    [[nodiscard]] observing::syncable observe_time_editor_for_time_presenter(
        std::function<void(std::shared_ptr<time_editor_for_time_presenter> const &)> &&) override;

   private:
    url const _editing_file_url;
    ae::file_info const _file_info;
    std::shared_ptr<player_for_project_editor> const _player;
    std::shared_ptr<file_track_for_project_editor> const _file_track;
    std::shared_ptr<marker_pool_for_project_editor> const _marker_pool;
    std::shared_ptr<edge_editor_for_project_editor> const _edge_editor;
    std::shared_ptr<pasteboard_for_project_editor> const _pasteboard;
    std::shared_ptr<database_for_project_editor> const _database;
    std::shared_ptr<exporter_for_project_editor> const _exporter;
    std::shared_ptr<action_controller> const _action_controller;
    std::shared_ptr<dialog_presenter> const _dialog_presenter;
    std::shared_ptr<nudging_for_project_editor> const _nudging;
    std::shared_ptr<timing_for_project_editor> const _timing;
    std::shared_ptr<time_editor_maker_for_project_editor> const _time_editor_maker;
    observing::value::holder_ptr<std::shared_ptr<time_editor_for_project_editor>> const _time_editor;

    proc::timeline_ptr const _timeline;
    proc::track_ptr _track;
    observing::canceller_pool _pool;
    observing::cancellable_ptr _time_editing_canceller;

    project_editor(
        url const &editing_file_url, ae::file_info const &, std::shared_ptr<player_for_project_editor> const &,
        std::shared_ptr<file_track_for_project_editor> const &, std::shared_ptr<marker_pool_for_project_editor> const &,
        std::shared_ptr<edge_editor_for_project_editor> const &, std::shared_ptr<pasteboard_for_project_editor> const &,
        std::shared_ptr<database_for_project_editor> const &, std::shared_ptr<exporter_for_project_editor> const &,
        std::shared_ptr<action_controller> const &, std::shared_ptr<dialog_presenter> const &,
        std::shared_ptr<nudging_for_project_editor> const &, std::shared_ptr<timing_for_project_editor> const &,
        std::shared_ptr<time_editor_maker_for_project_editor> const &);

    project_editor(project_editor const &) = delete;
    project_editor(project_editor &&) = delete;
    project_editor &operator=(project_editor const &) = delete;
    project_editor &operator=(project_editor &&) = delete;

    [[nodiscard]] std::optional<frame_index_t> _previous_jumpable_frame() const;
    [[nodiscard]] std::optional<frame_index_t> _next_jumpable_frame() const;
    [[nodiscard]] std::optional<frame_index_t> _first_edge() const;
    [[nodiscard]] std::optional<frame_index_t> _last_edge() const;
    [[nodiscard]] bool _can_editing() const;
};
}  // namespace yas::ae

//
//  ae_project_editor.h
//

#pragma once

#include <audio_editor_core/ae_project_editor_dependency.h>
#include <audio_editor_core/ae_project_id.h>
#include <audio_editor_core/ae_responder.h>

namespace yas::ae {
class sheet_presenter;
class dialog_presenter;
class timeline_updater;
class editing_status;

struct project_editor final {
    [[nodiscard]] static std::shared_ptr<project_editor> make_shared(
        project_id const &project_id, ae::file_info const &, std::shared_ptr<file_track_for_project_editor> const &,
        std::shared_ptr<marker_pool_for_project_editor> const &,
        std::shared_ptr<edge_holder_for_project_editor> const &, std::shared_ptr<pasteboard_for_project_editor> const &,
        std::shared_ptr<database_for_project_editor> const &, std::shared_ptr<exporter_for_project_editor> const &,
        std::shared_ptr<timing_for_project_editor> const &, std::shared_ptr<timeline_updater> const &,
        editing_status const *);

    void rotate_timing_fraction();

    [[nodiscard]] bool can_split() const;
    void split();
    void drop_head();
    void drop_tail();
    void drop_head_and_offset();
    void drop_tail_and_offset();

    [[nodiscard]] bool can_erase() const;
    void erase();
    void erase_and_offset();

    [[nodiscard]] bool can_return_to_zero() const;
    void return_to_zero();

    [[nodiscard]] bool can_go_to_marker(std::size_t const) const;
    void go_to_marker(std::size_t const);

    [[nodiscard]] bool can_undo() const;
    void undo();

    [[nodiscard]] bool can_redo() const;
    void redo();

    [[nodiscard]] bool can_select_file_for_export() const;
    void select_file_for_export();
    [[nodiscard]] bool can_export_to_file() const;
    void export_to_file(url const &);

    [[nodiscard]] bool can_cut() const;
    void cut_and_offset();
    [[nodiscard]] bool can_copy() const;
    void copy();
    [[nodiscard]] bool can_paste() const;
    void paste_and_offset();

    [[nodiscard]] bool can_begin_module_renaming() const;
    void begin_module_renaming(std::string const &range);

   private:
    identifier const _responder_id;
    ae::file_info const _file_info;
    std::shared_ptr<player_for_project_editor> const _player;
    std::shared_ptr<file_track_for_project_editor> const _file_track;
    std::shared_ptr<marker_pool_for_project_editor> const _marker_pool;
    std::shared_ptr<edge_holder_for_project_editor> const _edge_holder;
    std::shared_ptr<pasteboard_for_project_editor> const _pasteboard;
    std::shared_ptr<database_for_project_editor> const _database;
    std::shared_ptr<exporter_for_project_editor> const _exporter;
    std::shared_ptr<dialog_presenter> const _dialog_presenter;
    std::shared_ptr<sheet_presenter> const _sheet_presenter;
    std::shared_ptr<timing_for_project_editor> const _timing;
    std::shared_ptr<timeline_updater> const _timeline_updater;
    editing_status const *const _editing_status;

    observing::canceller_pool _pool;

    project_editor(ae::file_info const &, std::shared_ptr<player_for_project_editor> const &,
                   std::shared_ptr<file_track_for_project_editor> const &,
                   std::shared_ptr<marker_pool_for_project_editor> const &,
                   std::shared_ptr<edge_holder_for_project_editor> const &,
                   std::shared_ptr<pasteboard_for_project_editor> const &,
                   std::shared_ptr<database_for_project_editor> const &,
                   std::shared_ptr<exporter_for_project_editor> const &, std::shared_ptr<dialog_presenter> const &,
                   std::shared_ptr<sheet_presenter> const &, std::shared_ptr<timing_for_project_editor> const &,
                   std::shared_ptr<timeline_updater> const &, editing_status const *);

    project_editor(project_editor const &) = delete;
    project_editor(project_editor &&) = delete;
    project_editor &operator=(project_editor const &) = delete;
    project_editor &operator=(project_editor &&) = delete;

    [[nodiscard]] std::optional<frame_index_t> _previous_jumpable_frame() const;
    [[nodiscard]] std::optional<frame_index_t> _next_jumpable_frame() const;
    [[nodiscard]] std::optional<frame_index_t> _first_edge() const;
    [[nodiscard]] std::optional<frame_index_t> _last_edge() const;
};
}  // namespace yas::ae

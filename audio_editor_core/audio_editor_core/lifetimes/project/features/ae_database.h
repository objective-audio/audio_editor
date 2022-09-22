//
//  ae_database.h
//

#pragma once

#include <audio_editor_core/ae_common_types.h>
#include <audio_editor_core/ae_database_types.h>
#include <audio_editor_core/ae_db_types.h>
#include <audio_editor_core/ae_file_module.h>
#include <audio_editor_core/ae_file_ref_pool_dependencies.h>
#include <audio_editor_core/ae_file_track_dependencies.h>
#include <audio_editor_core/ae_marker.h>
#include <audio_editor_core/ae_marker_pool_dependencies.h>
#include <audio_editor_core/ae_pasteboard_types.h>
#include <cpp_utils/yas_delaying_caller.h>

#include <audio_editor_core/ae_file_ref.hpp>
#include <filesystem>

namespace yas::db {
class manager;
}  // namespace yas::db

namespace yas::ae {
struct database final : std::enable_shared_from_this<database>,
                        database_for_marker_pool,
                        database_for_file_track,
                        database_for_file_ref_pool {
    [[nodiscard]] static std::shared_ptr<database> make_shared(std::filesystem::path const &db_file_path);
    [[nodiscard]] static std::shared_ptr<database> make_shared(std::shared_ptr<db::manager> const &);

    [[nodiscard]] db_modules_map const &modules() const;
    [[nodiscard]] db_file_refs_map const &file_refs() const;
    [[nodiscard]] db_markers_map const &markers() const;
    [[nodiscard]] std::optional<db_edge> const &edge() const;
    [[nodiscard]] bool is_processing() const;

    [[nodiscard]] db_module add_module(file_module const &) override;
    void remove_module(time::range const &) override;
    void update_module(time::range const &, file_module const &) override;
    void update_module_detail(file_module const &);
    [[nodiscard]] db_marker add_marker(frame_index_t const frame, std::string const &name) override;
    void remove_marker(frame_index_t const &) override;
    void update_marker(frame_index_t const &prev_frame, marker_object const &) override;
    void set_edge(ae::edge const &);
    [[nodiscard]] std::optional<db_file_ref> add_file_ref(file_ref::params const &) override;

    void suspend_saving(std::function<void(void)> &&);

    void restore();

    [[nodiscard]] bool can_undo() const;
    void undo();

    [[nodiscard]] bool can_redo() const;
    void redo();

    [[nodiscard]] bool can_purge() const;
    void purge();

    [[nodiscard]] observing::endable observe(std::function<void(database_event const &)> &&);

   private:
    std::size_t _processing_count = 0;
    std::shared_ptr<db::manager> const _manager;
    db_modules_map _modules;
    db_file_refs_map _file_refs;
    db_markers_map _markers;
    std::optional<db_edge> _edge;
    delaying_caller _save_caller;
    observing::notifier_ptr<database_event> const _event_notifier;

    observing::canceller_pool _pool;

    database(std::shared_ptr<db::manager> const &);

    void _setup();
    void _increment_processing_count();
    void _decrement_processing_count();

    db::integer::type const &_current_save_id() const;
    db::integer::type const &_last_save_id() const;

    void _save();
    void _revert(db::integer::type const revert_id, bool const is_initial);
};
}  // namespace yas::ae

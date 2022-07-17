//
//  ae_database.h
//

#pragma once

#include <audio_editor_core/ae_common_types.h>
#include <audio_editor_core/ae_db_pasting_subject.h>
#include <audio_editor_core/ae_db_types.h>
#include <audio_editor_core/ae_file_module.h>
#include <audio_editor_core/ae_marker.h>
#include <audio_editor_core/ae_pasteboard_types.h>
#include <cpp_utils/yas_delaying_caller.h>
#include <cpp_utils/yas_url.h>

namespace yas::db {
class manager;
}  // namespace yas::db

namespace yas::ae {
struct database final : std::enable_shared_from_this<database> {
    [[nodiscard]] static std::shared_ptr<database> make_shared(url const &db_file_url);
    [[nodiscard]] static std::shared_ptr<database> make_shared(std::shared_ptr<db::manager> const &);

    [[nodiscard]] db_modules_map const &modules() const;
    [[nodiscard]] db_markers_map const &markers() const;
    [[nodiscard]] std::optional<db_edge> const &edge() const;
    [[nodiscard]] std::optional<pasting_value> pasting_value() const;
    [[nodiscard]] bool is_processing() const;

    void add_module(file_module const &);
    void remove_module(time::range const &);
    void update_module_detail(file_module const &);
    void set_pasting_value(std::optional<ae::pasting_value> const &);
    void add_marker(marker const &);
    void remove_marker(frame_index_t const &);
    void set_edge(ae::edge const &);

    void suspend_saving(std::function<void(void)> &&);

    void restore();

    [[nodiscard]] bool can_undo() const;
    void undo();

    [[nodiscard]] bool can_redo() const;
    void redo();

    [[nodiscard]] bool can_purge() const;
    void purge();

    [[nodiscard]] observing::endable observe_reverted(std::function<void(void)> &&);

   private:
    std::size_t _processing_count = 0;
    std::shared_ptr<db::manager> const _manager;
    db_modules_map _modules;
    db_markers_map _markers;
    std::optional<db_edge> _edge;
    std::optional<db_pasting_subject> _pasting_subject = std::nullopt;
    delaying_caller _save_caller;
    observing::notifier_ptr<std::nullptr_t> const _reverted_notifier;

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

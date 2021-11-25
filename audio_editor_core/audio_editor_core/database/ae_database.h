//
//  ae_database.h
//

#pragma once

#include <audio_editor_core/ae_db_types.h>
#include <audio_editor_core/ae_file_module.h>
#include <audio_editor_core/ae_marker.h>
#include <audio_editor_core/ae_project_editor_dependency.h>
#include <cpp_utils/yas_delaying_caller.h>
#include <cpp_utils/yas_url.h>

namespace yas::db {
class manager;
}  // namespace yas::db

namespace yas::ae {
struct database final : database_for_project_editor {
    [[nodiscard]] static std::shared_ptr<database> make_shared(url const &db_file_url);
    [[nodiscard]] static std::shared_ptr<database> make_shared(std::shared_ptr<db::manager> const &);

    [[nodiscard]] db_modules_map const &modules() const override;
    [[nodiscard]] db_markers_map const &markers() const override;
    [[nodiscard]] bool is_processing() const;

    void add_module(file_module const &) override;
    void remove_module(proc::time::range const &) override;
    void add_marker(marker const &) override;
    void remove_marker(proc::frame_index_t const &) override;

    void suspend_saving(std::function<void(void)> &&) override;

    [[nodiscard]] bool can_undo() const override;
    void undo() override;

    [[nodiscard]] bool can_redo() const override;
    void redo() override;

    [[nodiscard]] observing::endable observe_reverted(std::function<void(void)> &&) override;

   private:
    std::weak_ptr<database> _weak_database;
    std::size_t _processing_count = 0;
    std::shared_ptr<db::manager> const _manager;
    db_modules_map _modules;
    db_markers_map _markers;
    delaying_caller _save_caller;
    observing::notifier_ptr<std::nullptr_t> const _reverted_notifier;

    observing::canceller_pool _pool;

    database(std::shared_ptr<db::manager> const &);

    void _setup(std::weak_ptr<database>);
    void _increment_processing_count();
    void _decrement_processing_count();

    db::integer::type const &_current_save_id() const;
    db::integer::type const &_last_save_id() const;

    void _save();
    void _revert(db::integer::type const revert_id);
};
}  // namespace yas::ae

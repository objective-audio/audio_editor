//
//  ae_reverter.h
//

#pragma once

#include <observing/yas_observing_umbrella.h>

namespace yas::ae {
class database;
class file_track;
class marker_pool;
class pasteboard;
class edge_holder;
class selected_file_module_pool;
class editing_status;
class project_path;

struct reverter final {
    reverter(project_path const *, database *, file_track *, marker_pool *, pasteboard *, edge_holder *,
             selected_file_module_pool *, editing_status const *);

    [[nodiscard]] bool can_undo() const;
    void undo();

    [[nodiscard]] bool can_redo() const;
    void redo();

    [[nodiscard]] bool can_purge() const;
    void purge();

   private:
    project_path const *const _project_path;
    database *const _database;
    file_track *const _file_track;
    marker_pool *const _marker_pool;
    pasteboard *const _pasteboard;
    edge_holder *const _edge_holder;
    selected_file_module_pool *const _selected_file_module_pool;
    editing_status const *const _editing_status;

    observing::canceller_pool _pool;

    reverter(reverter const &) = delete;
    reverter(reverter &&) = delete;
    reverter &operator=(reverter const &) = delete;
    reverter &operator=(reverter &&) = delete;
};
}  // namespace yas::ae

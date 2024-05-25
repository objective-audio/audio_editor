//
//  reverter.cpp
//

#include "reverter.h"

#include <ae-core/project/features/project_path.h>
#include <ae-core/project_editing/features/database.h>
#include <ae-core/project_editing/features/edge_holder.h>
#include <ae-core/project_editing/features/editing_status.h>
#include <ae-core/project_editing/features/marker_pool.h>
#include <ae-core/project_editing/features/module_pool.h>
#include <ae-core/project_editing/features/pasteboard.h>
#include <cpp-utils/assertion.h>
#include <cpp-utils/file_manager.h>

using namespace yas;
using namespace yas::ae;

reverter::reverter(project_path const *project_path, database *database, module_pool *module_pool,
                   marker_pool *marker_pool, pasteboard *pasteboard, edge_holder *edge_holder,
                   selected_module_pool *selected_pool, editing_status const *editing_status)
    : _project_path(project_path),
      _database(database),
      _module_pool(module_pool),
      _marker_pool(marker_pool),
      _pasteboard(pasteboard),
      _edge_holder(edge_holder),
      _selected_module_pool(selected_pool),
      _editing_status(editing_status) {
    this->_database
        ->observe([this](database_event const &event) {
            switch (event) {
                case database_event::reverted: {
                    std::vector<module_object> modules;

                    for (auto const &pair : this->_database->modules()) {
                        if (auto object = pair.second.object()) {
                            modules.emplace_back(std::move(object.value()));
                        }
                    }

                    this->_module_pool->revert_modules_and_notify(std::move(modules));

                    std::vector<marker_object> markers;

                    for (auto const &pair : this->_database->markers()) {
                        if (auto const marker = pair.second.object()) {
                            markers.emplace_back(std::move(marker.value()));
                        }
                    }

                    this->_marker_pool->revert(std::move(markers));

                    if (auto const &db_edge = this->_database->edge()) {
                        this->_edge_holder->revert_edge(db_edge.value().edge());
                    } else {
                        this->_edge_holder->revert_edge(ae::edge::zero());
                    }

                    this->_pasteboard->clear();
                    this->_selected_module_pool->clear();
                } break;

                case database_event::purged: {
                    std::set<std::string> file_names;

                    for (auto const &pair : this->_module_pool->modules()) {
                        file_names.emplace(pair.second.value.file_name);
                    }

                    auto const directory = this->_project_path->editing_files_directory();

                    if (file_manager::content_exists(directory)) {
                        auto const paths_result = file_manager::content_paths_in_directory(directory);

                        if (paths_result.is_error()) {
                            assertion_failure_if_not_test();
                            return;
                        }

                        for (std::filesystem::path const &path : paths_result.value()) {
                            if (path.has_filename() && !file_names.contains(path.filename())) {
                                auto const result = file_manager::remove_content(path);

                                if (result.is_error()) {
                                    assertion_failure_if_not_test();
                                }
                            }
                        }
                    }
                } break;
            }
        })
        .end()
        ->add_to(this->_pool);
}

bool reverter::can_undo() const {
    if (!this->_editing_status->can_editing()) {
        return false;
    }

    return this->_database->can_undo();
}

void reverter::undo() {
    if (!this->can_undo()) {
        return;
    }

    this->_database->undo();
}

bool reverter::can_redo() const {
    if (!this->_editing_status->can_editing()) {
        return false;
    }

    return this->_database->can_redo();
}

void reverter::redo() {
    if (!this->can_redo()) {
        return;
    }

    this->_database->redo();
}

bool reverter::can_purge() const {
    return this->_database->can_purge();
}

void reverter::purge() {
    this->_database->purge();
}

//
//  ae_reverter.cpp
//

#include "ae_reverter.h"

#include <audio_editor_core/ae_database.h>
#include <audio_editor_core/ae_edge_holder.h>
#include <audio_editor_core/ae_editing_status.h>
#include <audio_editor_core/ae_file_track.h>
#include <audio_editor_core/ae_marker_pool.h>
#include <audio_editor_core/ae_pasteboard.h>

#include <audio_editor_core/ae_file_ref_pool.hpp>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<reverter> reverter::make_shared(database *database, file_track *file_track, marker_pool *marker_pool,
                                                file_ref_pool *file_ref_pool, pasteboard *pasteboard,
                                                edge_holder *edge_holder, editing_status const *editing_status) {
    return std::make_shared<reverter>(database, file_track, marker_pool, file_ref_pool, pasteboard, edge_holder,
                                      editing_status);
}

reverter::reverter(database *database, file_track *file_track, marker_pool *marker_pool, file_ref_pool *file_ref_pool,
                   pasteboard *pasteboard, edge_holder *edge_holder, editing_status const *editing_status)
    : _database(database),
      _file_track(file_track),
      _marker_pool(marker_pool),
      _file_ref_pool(file_ref_pool),
      _pasteboard(pasteboard),
      _edge_holder(edge_holder),
      _editing_status(editing_status) {
    this->_database
        ->observe_reverted([this] {
            std::vector<file_module> file_modules;

            for (auto const &pair : this->_database->modules()) {
                if (auto file_module = pair.second.file_module()) {
                    file_modules.emplace_back(std::move(file_module.value()));
                }
            }

            this->_file_track->revert_modules_and_notify(std::move(file_modules));

            std::vector<marker> markers;

            for (auto const &pair : this->_database->markers()) {
                if (auto const marker = pair.second.marker()) {
                    markers.emplace_back(std::move(marker.value()));
                }
            }

            this->_marker_pool->revert_markers(std::move(markers));

            std::vector<file_ref> file_refs;

            for (auto const &pair : this->_database->file_refs()) {
                if (auto const file_ref = pair.second.file_ref()) {
                    file_refs.emplace_back(std::move(file_ref.value()));
                }
            }

            this->_file_ref_pool->revert(std::move(file_refs));

            if (auto const &db_edge = this->_database->edge()) {
                this->_edge_holder->revert_edge(db_edge.value().edge());
            } else {
                this->_edge_holder->revert_edge(ae::edge::zero());
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

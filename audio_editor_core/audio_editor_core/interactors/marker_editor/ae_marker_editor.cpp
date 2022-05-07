//
//  ae_marker_editor.cpp
//

#include "ae_marker_editor.h"

#include <audio_editor_core/ae_database.h>
#include <audio_editor_core/ae_exporter.h>
#include <audio_editor_core/ae_hierarchy.h>
#include <audio_editor_core/ae_marker_pool.h>
#include <audio_editor_core/ae_player.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<marker_editor> marker_editor::make_shared(project_id const &project_id, marker_pool *marker_pool,
                                                          database *database, exporter const *exporter) {
    auto const &project_level = hierarchy::project_level_for_id(project_id);
    return std::shared_ptr<marker_editor>(
        new marker_editor{project_level->player.get(), marker_pool, database, exporter});
}

marker_editor::marker_editor(player const *player, marker_pool *marker_pool, database *database,
                             exporter const *exporter)
    : _player(player), _marker_pool(marker_pool), _database(database), _exporter(exporter) {
}

bool marker_editor::can_insert_marker() const {
    if (!this->_can_editing()) {
        return false;
    }

    auto const current_frame = this->_player->current_frame();
    return this->_marker_pool->markers().count(current_frame) == 0;
}

void marker_editor::insert_marker() {
    if (!this->can_insert_marker()) {
        return;
    }

    this->_database->suspend_saving([this] {
        auto const current_frame = this->_player->current_frame();
        this->_marker_pool->insert_marker(marker{.frame = current_frame});
    });
}

bool marker_editor::_can_editing() const {
    return !this->_exporter->is_exporting();
}

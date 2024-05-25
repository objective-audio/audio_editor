//
//  export_interactor.cpp
//

#include "export_interactor.h"

#include <ae-core/global/hierarchy.h>
#include <ae-core/project/features/player.h>
#include <ae-core/project/features/timeline_holder.h>
#include <ae-core/project_editing/features/edge_holder.h>
#include <ae-core/project_editing/features/editing_status.h>
#include <ae-core/project_editing/features/exporter.h>
#include <ae-core/project_editing/lifecycles/project_modal_lifecycle.h>

using namespace yas;
using namespace yas::ae;

export_interactor::export_interactor(project_format const &project_format, project_modal_lifecycle *lifecycle,
                                     editing_status const *editing_status, edge_holder const *edge_holder,
                                     player *player, exporter *exporter, timeline_holder const *timeline_holder)
    : _project_format(project_format),
      _lifecycle(lifecycle),
      _editing_status(editing_status),
      _edge_holder(edge_holder),
      _player(player),
      _exporter(exporter),
      _timeline_holder(timeline_holder) {
}

bool export_interactor::can_select_file_for_export() const {
    return this->can_export_to_file() && !this->_lifecycle->current().has_value();
}

void export_interactor::select_file_for_export() {
    if (!this->can_select_file_for_export()) {
        return;
    }

    this->_lifecycle->add_file_export_dialog();
}

bool export_interactor::can_export_to_file() const {
    if (!this->_editing_status->can_editing()) {
        return false;
    }

    return this->_edge_holder->edge().range().has_value();
}

void export_interactor::export_to_file(std::filesystem::path const &export_path) {
    if (!this->can_export_to_file()) {
        return;
    }

    auto const range = this->_edge_holder->edge().range();
    if (!range.has_value()) {
        return;
    }

    this->_player->set_playing(false);

    exporting_format const format{.sample_rate = this->_project_format.sample_rate,
                                  .pcm_format = audio::pcm_format::float32,
                                  .channel_count = this->_project_format.channel_count};

    this->_exporter->begin(export_path, this->_timeline_holder->timeline(), format, range.value());
}

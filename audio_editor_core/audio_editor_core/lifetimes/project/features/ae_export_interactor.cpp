//
//  ae_export_interactor.cpp
//

#include "ae_export_interactor.h"

#include <audio_editor_core/ae_edge_holder.h>
#include <audio_editor_core/ae_editing_status.h>
#include <audio_editor_core/ae_exporter.h>
#include <audio_editor_core/ae_hierarchy.h>
#include <audio_editor_core/ae_player.h>
#include <audio_editor_core/ae_project_modal_lifecycle.h>
#include <audio_editor_core/ae_timeline_holder.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<export_interactor> export_interactor::make_shared(
    project_format const &project_format, project_modal_lifecycle *lifecycle, editing_status const *editing_status,
    edge_holder const *edge_holder, player *player, exporter *exporter, timeline_holder const *timeline_holder) {
    return std::make_shared<export_interactor>(project_format, lifecycle, editing_status, edge_holder, player, exporter,
                                               timeline_holder);
}

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

    this->_lifecycle->add_dialog(dialog_content::select_file_for_export);
}

bool export_interactor::can_export_to_file() const {
    if (!this->_editing_status->can_editing()) {
        return false;
    }

    return this->_edge_holder->edge().range().has_value();
}

void export_interactor::export_to_file(url const &export_url) {
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

    this->_exporter->begin(export_url, this->_timeline_holder->timeline(), format, range.value());
}

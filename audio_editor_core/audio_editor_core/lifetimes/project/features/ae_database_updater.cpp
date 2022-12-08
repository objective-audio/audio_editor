//
//  ae_database_updater.cpp
//

#include "ae_database_updater.h"

#include <audio_editor_core/ae_database.h>
#include <audio_editor_core/ae_edge_holder.h>
#include <audio_editor_core/ae_file_track.h>
#include <audio_editor_core/ae_marker_pool.h>
#include <audio_editor_core/ae_pasteboard.h>
#include <cpp_utils/yas_assertion.h>

using namespace yas;
using namespace yas::ae;

database_updater::database_updater(edge_holder *edge_holder, database *database)
    : _edge_holder(edge_holder), _database(database) {
    this->_edge_holder
        ->observe_event([this](edge_holder_event const &event) {
            switch (event.type) {
                case edge_holder_event_type::updated:
                    this->_database->set_edge(event.edge);
                    break;

                case edge_holder_event_type::fetched:
                case edge_holder_event_type::reverted:
                    break;
            }
        })
        .end()
        ->add_to(this->_pool);
}

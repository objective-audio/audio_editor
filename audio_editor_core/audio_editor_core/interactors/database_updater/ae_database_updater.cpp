//
//  ae_database_updater.cpp
//

#include "ae_database_updater.h"

#include <audio_editor_core/ae_database.h>
#include <audio_editor_core/ae_edge_holder.h>
#include <audio_editor_core/ae_file_track.h>
#include <audio_editor_core/ae_marker_pool.h>
#include <audio_editor_core/ae_pasteboard.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<database_updater> database_updater::make_shared(file_track *file_track, marker_pool *marker_pool,
                                                                edge_holder *edge_holder, pasteboard *pasteboard,
                                                                database *database) {
    return std::shared_ptr<database_updater>(
        new database_updater{file_track, marker_pool, edge_holder, pasteboard, database});
}

database_updater::database_updater(file_track *file_track, marker_pool *marker_pool, edge_holder *edge_holder,
                                   pasteboard *pasteboard, database *database)
    : _file_track(file_track),
      _marker_pool(marker_pool),
      _edge_holder(edge_holder),
      _pasteboard(pasteboard),
      _database(database) {
    this->_file_track
        ->observe_event([this](file_track_event const &event) {
            switch (event.type) {
                case file_track_event_type::any: {
                    for (auto const &pair : event.modules) {
                        auto const &file_module = pair.second;
                        this->_database->add_module(file_module);
                    }
                } break;
                case file_track_event_type::reverted:
                    break;
                case file_track_event_type::inserted: {
                    auto const &file_module = event.module.value();
                    this->_database->add_module(file_module);
                } break;
                case file_track_event_type::erased: {
                    auto const &range = event.module.value().range;
                    this->_database->remove_module(range);
                } break;
                case file_track_event_type::detail_updated: {
                    auto const &file_module = event.module.value();
                    this->_database->update_module_detail(file_module);
                } break;
            }
        })
        .sync()
        ->add_to(this->_pool);

    this->_marker_pool
        ->observe_event([this](marker_pool_event const &event) {
            switch (event.type) {
                case marker_pool_event_type::any:
                    if (event.markers.size() > 0) {
                        for (auto const &pair : event.markers) {
                            this->_database->add_marker(pair.second);
                        }
                    }
                    break;
                case marker_pool_event_type::reverted:
                    break;
                case marker_pool_event_type::inserted:
                    this->_database->add_marker(event.marker.value());
                    break;
                case marker_pool_event_type::erased:
                    this->_database->remove_marker(event.marker.value().frame);
                    break;
            }
        })
        .sync()
        ->add_to(this->_pool);

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
        .sync()
        ->add_to(this->_pool);

    this->_pasteboard
        ->observe_event([this](pasteboard_event const &event) {
            switch (event) {
                case pasteboard_event::file_module:
                    this->_database->set_pasting_data(this->_pasteboard->data());
                    break;
                case pasteboard_event::fetched:
                case pasteboard_event::reverted:
                    break;
            }
        })
        .sync()
        ->add_to(this->_pool);
}

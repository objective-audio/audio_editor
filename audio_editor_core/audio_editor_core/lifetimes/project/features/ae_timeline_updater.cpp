//
//  ae_timeline_updater.cpp
//

#include "ae_timeline_updater.h"

#include <audio_editor_core/ae_file_track.h>
#include <audio_editor_core/ae_timeline_holder.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<timeline_updater> timeline_updater::make_shared(file_track *file_track,
                                                                timeline_holder *timeline_holder) {
    return std::make_shared<timeline_updater>(file_track, timeline_holder);
}

timeline_updater::timeline_updater(file_track *file_track, timeline_holder *timeline_holder)
    : _file_track(file_track), _timeline_holder(timeline_holder) {
    this->_file_track
        ->observe_event([this](file_track_event const &event) {
            switch (event.type) {
                case file_track_event_type::any: {
                    this->_timeline_holder->replace(event.modules);
                } break;
                case file_track_event_type::reverted: {
                    this->_timeline_holder->replace(event.modules);
                } break;
                case file_track_event_type::inserted: {
                    auto const &file_module = event.module.value().value;
                    this->_timeline_holder->insert(file_module);
                } break;
                case file_track_event_type::erased: {
                    auto const &range = event.module.value().value.range;
                    this->_timeline_holder->erase(range);
                } break;
                case file_track_event_type::detail_updated:
                    break;
            }
        })
        .sync()
        ->add_to(this->_pool);
}

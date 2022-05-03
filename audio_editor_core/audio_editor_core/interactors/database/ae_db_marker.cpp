//
//  ae_db_marker.cpp
//

#include "ae_db_marker.h"

#include <audio_editor_core/ae_db_constants.h>
#include <audio_editor_core/ae_marker.h>

using namespace yas;
using namespace yas::ae;
using namespace yas::ae::db_constants;

db_marker::db_marker(db::object_ptr const &object) : _object(object) {
}

db_marker db_marker::create(db::manager_ptr const &manager, ae::marker const &marker) {
    auto object = manager->create_object(marker_name::entity);
    object->set_attribute_value(marker_name::attribute::frame, db::value{marker.frame});
    return db_marker{object};
}

std::optional<marker> db_marker::marker() const {
    if (auto const &frame_value = this->_object->attribute_value(marker_name::attribute::frame)) {
        return ae::marker{.frame = frame_value.get<db::integer>()};
    }
    return std::nullopt;
}

void db_marker::remove() {
    this->_object->remove();
}

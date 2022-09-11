//
//  ae_db_marker.cpp
//

#include "ae_db_marker.h"

#include <audio_editor_core/ae_db_marker_constants.h>
#include <audio_editor_core/ae_marker.h>
#include <cpp_utils/yas_assertion.h>

using namespace yas;
using namespace yas::ae;
using namespace yas::ae::db_constants;

db_marker::db_marker(db::object_ptr const &object) : _object(object) {
}

db_marker db_marker::create(db::manager_ptr const &manager, frame_index_t const frame, std::string const &name) {
    auto object = manager->create_object(marker_name::entity);
    object->set_attribute_value(marker_name::attribute::frame, db::value{frame});
    object->set_attribute_value(marker_name::attribute::name, db::value{name});
    return db_marker{object};
}

std::optional<marker> db_marker::marker() const {
    auto const &frame_value = this->_object->attribute_value(marker_name::attribute::frame);
    auto const &name_value = this->_object->attribute_value(marker_name::attribute::name);

    if (frame_value && name_value) {
        return ae::marker{this->_object->object_id(), frame_value.get<db::integer>(), name_value.get<db::text>()};
    }

    return std::nullopt;
}

void db_marker::set_marker(ae::marker const &marker) {
    if (marker.identifier != this->_object->object_id()) {
        return;
    }

    this->_object->set_attribute_value(marker_name::attribute::frame, db::value{marker.frame});
    this->_object->set_attribute_value(marker_name::attribute::name, db::value{marker.name});
}

void db_marker::remove() {
    this->_object->remove();
}

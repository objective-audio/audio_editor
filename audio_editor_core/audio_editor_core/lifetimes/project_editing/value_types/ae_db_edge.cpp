//
//  ae_db_edge.cpp
//

#include "ae_db_edge.h"

#include <audio_editor_core/ae_db_edge_constants.h>

using namespace yas;
using namespace yas::ae;
using namespace yas::ae::db_constants;

db_edge::db_edge(db::object_ptr const &object) : _object(object) {
}

db_edge db_edge::create(db::manager_ptr const &manager, ae::edge const &value) {
    auto object = manager->create_object(edge_name::entity);
    object->set_attribute_value(edge_name::attribute::begin, db::value{value.begin_frame});
    object->set_attribute_value(edge_name::attribute::end, db::value{value.end_frame});
    return db_edge{object};
}

ae::edge db_edge::edge() const {
    auto const &begin = this->_object->attribute_value(edge_name::attribute::begin);
    auto const &end = this->_object->attribute_value(edge_name::attribute::end);

    return ae::edge{begin.get<db::integer>(), end.get<db::integer>()};
}

void db_edge::set(ae::edge const &value) {
    this->_object->set_attribute_value(edge_name::attribute::begin, db::value{value.begin_frame});
    this->_object->set_attribute_value(edge_name::attribute::end, db::value{value.end_frame});
}

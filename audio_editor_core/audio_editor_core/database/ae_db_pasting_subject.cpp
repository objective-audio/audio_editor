//
//  ae_db_pasting_subject.cpp
//

#include "ae_db_pasting_subject.h"

#include <audio_editor_core/ae_db_constants.h>

using namespace yas;
using namespace yas::ae;
using namespace yas::ae::db_constants;

db_pasting_subject::db_pasting_subject(db::object_ptr const &object) : _object(object) {
}

db_pasting_subject db_pasting_subject::create(db::manager_ptr const &manager, std::string const &data) {
    auto object = manager->create_object(pasting_subject_name::entity);
    object->set_attribute_value(pasting_subject_name::attribute::data, db::value{data});
    return db_pasting_subject{std::move(object)};
}

std::string db_pasting_subject::data() const {
    if (auto const &value = this->_object->attribute_value(pasting_subject_name::attribute::data)) {
        return value.get<db::text>();
    } else {
        return "";
    }
}

void db_pasting_subject::remove() {
    this->_object->remove();
}

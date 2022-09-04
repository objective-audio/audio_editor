//
//  ae_db_file_reference.cpp
//

#include "ae_db_file_reference.hpp"

#include <audio_editor_core/ae_db_file_reference_constants.h>

#include <audio_editor_core/ae_file_reference.hpp>

using namespace yas;
using namespace yas::ae;
using namespace yas::ae::db_constants;

db_file_reference::db_file_reference(db::object_ptr const &object) : _object(object) {
}

db_file_reference db_file_reference::create(db::manager_ptr const &manager, ae::file_reference const &file_reference) {
    auto object = manager->create_object(file_reference_name::entity);
    object->set_attribute_value(file_reference_name::attribute::file_name, db::value{file_reference.file_name});
    return db_file_reference{std::move(object)};
}

std::optional<file_reference> db_file_reference::file_ref() const {
    auto const &file_name_value = this->_object->attribute_value(file_reference_name::attribute::file_name);

    if (file_name_value) {
        auto const file_name = file_name_value.get<db::text>();
        return ae::file_reference{.file_name = file_name};
    }

    return std::nullopt;
}

void db_file_reference::remove() {
    this->_object->remove();
}

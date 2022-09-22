//
//  ae_db_file_ref.cpp
//

#include "ae_db_file_ref.hpp"

#include <audio_editor_core/ae_db_file_ref_constants.h>

#include <audio_editor_core/ae_file_ref.hpp>

using namespace yas;
using namespace yas::ae;
using namespace yas::ae::db_constants;

db_file_ref::db_file_ref(db::object_ptr const &object) : _object(object) {
}

db_file_ref db_file_ref::create(db::manager_ptr const &manager, ae::file_ref const &file_ref) {
    auto object = manager->create_object(file_ref_name::entity);
    object->set_attribute_value(file_ref_name::attribute::file_name, db::value{file_ref.file_name});
    return db_file_ref{std::move(object)};
}

std::optional<file_ref_object> db_file_ref::file_ref() const {
    auto const &file_name_value = this->_object->attribute_value(file_ref_name::attribute::file_name);

    if (file_name_value) {
        auto const file_name = file_name_value.get<db::text>();
        return ae::file_ref_object{this->_object->object_id(), file_name};
    }

    return std::nullopt;
}

void db_file_ref::remove() {
    this->_object->remove();
}

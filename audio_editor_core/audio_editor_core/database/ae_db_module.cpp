//
//  ae_db_module.cpp
//

#include "ae_db_module.h"

#include <audio_editor_core/ae_db_constants.h>
#include <audio_editor_core/ae_file_module.h>

using namespace yas;
using namespace yas::ae;

db_module::db_module(db::object_ptr const &object) : _object(object) {
}

db_module db_module::create(db::manager_ptr const &manager, ae::file_module const &file_module) {
    auto object = manager->create_object(db_constants::module_name::entity);
    object->set_attribute_value(db_constants::module_name::file_frame, db::value{file_module.file_frame});
    object->set_attribute_value(db_constants::module_name::range_frame, db::value{file_module.range.frame});
    object->set_attribute_value(db_constants::module_name::range_length, db::value{file_module.range.length});
    return db_module{std::move(object)};
}

std::optional<file_module> db_module::file_module() const {
    auto const file_frame_value = this->_object->attribute_value(db_constants::module_name::file_frame);
    auto const range_frame_value = this->_object->attribute_value(db_constants::module_name::range_frame);
    auto const range_length_value = this->_object->attribute_value(db_constants::module_name::range_length);

    if (file_frame_value && range_frame_value && range_length_value) {
        if (auto const range_length = static_cast<proc::length_t>(range_length_value.get<db::integer>());
            range_length > 0) {
            auto const file_frame = file_frame_value.get<db::integer>();
            auto const range_frame = range_frame_value.get<db::integer>();
            return ae::file_module{.range = proc::time::range{range_frame, range_length}, .file_frame = file_frame};
        }
    }

    return std::nullopt;
}

void db_module::remove() {
    this->_object->remove();
}

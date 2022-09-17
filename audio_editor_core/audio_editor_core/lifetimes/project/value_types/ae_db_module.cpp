//
//  ae_db_module.cpp
//

#include "ae_db_module.h"

#include <audio_editor_core/ae_db_module_constants.h>
#include <audio_editor_core/ae_file_module.h>

using namespace yas;
using namespace yas::ae;
using namespace yas::ae::db_constants;

db_module::db_module(db::object_ptr const &object) : _object(object) {
}

db_module db_module::create(db::manager_ptr const &manager, file_module::params const &params) {
    auto object = manager->create_object(module_name::entity);
    object->set_attribute_value(module_name::attribute::name, db::value{params.name});
    object->set_attribute_value(module_name::attribute::file_frame, db::value{params.file_frame});
    object->set_attribute_value(module_name::attribute::range_frame, db::value{params.range.frame});
    object->set_attribute_value(module_name::attribute::range_length, db::value{params.range.length});
    object->set_attribute_value(module_name::attribute::file_name, db::value{params.file_name});
    return db_module{std::move(object)};
}

std::optional<file_module> db_module::file_module() const {
    auto const &name_value = this->_object->attribute_value(module_name::attribute::name);
    auto const &file_frame_value = this->_object->attribute_value(module_name::attribute::file_frame);
    auto const &range_frame_value = this->_object->attribute_value(module_name::attribute::range_frame);
    auto const &range_length_value = this->_object->attribute_value(module_name::attribute::range_length);
    auto const &file_name_value = this->_object->attribute_value(module_name::attribute::file_name);

    if (name_value && file_frame_value && range_frame_value && range_length_value && file_name_value) {
        if (auto const range_length = static_cast<proc::length_t>(range_length_value.get<db::integer>());
            range_length > 0) {
            auto name = name_value.get<db::text>();
            auto const file_frame = file_frame_value.get<db::integer>();
            auto const range_frame = range_frame_value.get<db::integer>();
            auto const file_name = file_name_value.get<db::text>();
            return ae::file_module{this->_object->object_id(), std::move(name), time::range{range_frame, range_length},
                                   file_frame, file_name};
        }
    }

    return std::nullopt;
}

void db_module::set_range(time::range const &range) {
    this->_object->set_attribute_value(module_name::attribute::range_frame, db::value{range.frame});
    this->_object->set_attribute_value(module_name::attribute::range_length, db::value{range.length});
}

void db_module::set_name(std::string const &name) {
    this->_object->set_attribute_value(module_name::attribute::name, db::value{name});
}

void db_module::remove() {
    this->_object->remove();
}

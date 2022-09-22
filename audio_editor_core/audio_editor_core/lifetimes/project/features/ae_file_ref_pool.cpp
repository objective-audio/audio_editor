//
//  ae_file_ref_pool.cpp
//

#include "ae_file_ref_pool.hpp"

#include <cpp_utils/yas_assertion.h>
#include <cpp_utils/yas_stl_utils.h>

#include <audio_editor_core/ae_db_file_ref.hpp>

using namespace yas;
using namespace yas::ae;

file_ref_pool::file_ref_pool(database_for_file_ref_pool *database)
    : _database(database), _refs(observing::map::holder<std::string, file_ref_object>::make_shared()) {
}

void file_ref_pool::revert(std::vector<file_ref_object> &&refs) {
    this->_refs->replace(
        to_map<std::string>(std::move(refs), [](file_ref_object const &ref) { return ref.file_name; }));
}

void file_ref_pool::insert(file_ref_object::params const &file_ref) {
    if (!this->_refs->contains(file_ref.file_name)) {
        if (auto const db_ref = this->_database->add_file_ref(file_ref); db_ref.has_value()) {
            if (auto const object = db_ref.value().file_ref(); object.has_value()) {
                this->_refs->insert_or_replace(file_ref.file_name, object.value());
            }
        }
    }
    assertion_failure_if_not_test();
}

std::optional<file_ref_object> file_ref_pool::ref(std::string const &file_name) const {
    if (this->_refs->contains(file_name)) {
        return this->_refs->at(file_name);
    } else {
        return std::nullopt;
    }
}

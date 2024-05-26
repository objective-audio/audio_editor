//
//  module_name_editor.cpp
//

#include "module_name_editor.hpp"

#include <ae-core/global/hierarchy.h>
#include <ae-core/project_editing/features/database.h>
#include <ae-core/project_editing/features/module_pool.h>

using namespace yas;
using namespace yas::ae;

static std::string const empty_string = "";

std::shared_ptr<module_name_editor> module_name_editor::make_shared(project_sub_lifetime_id const &lifetime_id,
                                                                    module_index const &index) {
    auto const &lifetime = hierarchy::project_editing_lifetime_for_id(lifetime_id.project);
    return std::make_shared<module_name_editor>(index, lifetime->module_pool.get(), lifetime->database.get());
}

module_name_editor::module_name_editor(module_index const &index, module_pool *module_pool, database *database)
    : _module_index(index), _module_pool(module_pool), _database(database) {
}

std::string const &module_name_editor::name() const {
    auto const &modules = this->_module_pool->modules();
    if (modules.contains(this->_module_index)) {
        return modules.at(this->_module_index).value.name;
    }
    return empty_string;
}

void module_name_editor::set_name(std::string const &name) {
    this->_module_pool->set_module_name_and_notify(this->_module_index, name);
}

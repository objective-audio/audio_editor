//
//  ae_module_name_editor.cpp
//

#include "ae_module_name_editor.hpp"

#include <audio_editor_core/ae_database.h>
#include <audio_editor_core/ae_file_track.h>
#include <audio_editor_core/ae_hierarchy.h>

using namespace yas;
using namespace yas::ae;

static std::string const empty_string = "";

std::shared_ptr<module_name_editor> module_name_editor::make_shared(project_sub_lifetime_id const &lifetime_id,
                                                                    file_module_index const &index) {
    auto const &project_lifetime = hierarchy::project_lifetime_for_id(lifetime_id.window);
    return std::make_shared<module_name_editor>(index, project_lifetime->file_track.get(),
                                                project_lifetime->database.get());
}

module_name_editor::module_name_editor(file_module_index const &index, file_track *file_track, database *database)
    : _module_index(index), _file_track(file_track), _database(database) {
}

std::string const &module_name_editor::name() const {
    auto const &modules = this->_file_track->modules();
    if (modules.contains(this->_module_index)) {
        return modules.at(this->_module_index).value.name;
    }
    return empty_string;
}

void module_name_editor::set_name(std::string const &name) {
    this->_file_track->set_module_name_and_notify(this->_module_index, name);
}

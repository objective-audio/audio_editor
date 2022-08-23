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

std::shared_ptr<module_name_editor> module_name_editor::make_shared(sheet_lifetime_id const &lifetime_id,
                                                                    time::range const &range) {
    auto const &project_lifetime = hierarchy::project_lifetime_for_id(lifetime_id.window);
    return std::make_shared<module_name_editor>(range, project_lifetime->file_track.get(),
                                                project_lifetime->database.get());
}

module_name_editor::module_name_editor(time::range const &range, file_track *file_track, database *database)
    : _module_range(range), _file_track(file_track), _database(database) {
}

std::string const &module_name_editor::name() const {
    auto const &modules = this->_file_track->modules();
    if (modules.contains(this->_module_range)) {
        return modules.at(this->_module_range).name;
    }
    return empty_string;
}

void module_name_editor::set_name(std::string const &name) {
    this->_database->suspend_saving(
        [this, &name] { this->_file_track->set_module_name_and_notify(this->_module_range, name); });
}

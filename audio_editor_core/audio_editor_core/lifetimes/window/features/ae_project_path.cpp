//
//  ae_project_path.cpp
//

#include "ae_project_path.h"

using namespace yas;
using namespace yas::ae;

project_path::project_path(std::filesystem::path const &root) : _root(root) {
}

std::filesystem::path const &project_path::root_directory() const {
    return this->_root;
}

std::filesystem::path project_path::editing_files_directory() const {
    auto root = this->_root;
    return root.append("editing_files");
}

std::filesystem::path project_path::editing_file() const {
    auto root = this->_root;
    return root.append("editing.caf");
}

std::filesystem::path project_path::playing_directory() const {
    auto root = this->_root;
    return root.append("playing");
}

std::filesystem::path project_path::db_file() const {
    auto root = this->_root;
    return root.append("db.sqlite");
}

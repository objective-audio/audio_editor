//
//  ae_project_url.cpp
//

#include "ae_project_url.h"

using namespace yas;
using namespace yas::ae;

std::shared_ptr<project_url> project_url::make_shared(std::filesystem::path const &root) {
    return std::shared_ptr<project_url>(new project_url{root});
}

project_url::project_url(std::filesystem::path const &root) : _root(root) {
}

std::filesystem::path const &project_url::root_directory() const {
    return this->_root;
}

std::filesystem::path project_url::editing_files_directory() const {
    auto root = this->_root;
    return root.append("editing_files");
}

std::filesystem::path project_url::editing_file() const {
    auto root = this->_root;
    return root.append("editing.caf");
}

std::filesystem::path project_url::playing_directory() const {
    auto root = this->_root;
    return root.append("playing");
}

std::filesystem::path project_url::db_file() const {
    auto root = this->_root;
    return root.append("db.sqlite");
}

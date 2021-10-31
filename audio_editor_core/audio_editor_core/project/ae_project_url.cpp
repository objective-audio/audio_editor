//
//  ae_project_url.cpp
//

#include "ae_project_url.h"

using namespace yas;
using namespace yas::ae;

project_url::project_url(url const &root) : _root(root) {
}

url const &project_url::root_directory() const {
    return this->_root;
}

url project_url::editing_file() const {
    return this->_root.appending("editing.caf");
}

url project_url::playing_directory() const {
    return this->_root.appending("playing");
}

url project_url::db_file() const {
    return this->_root.appending("db.sqlite");
}

std::shared_ptr<project_url> project_url::make_shared(url const &root) {
    return std::shared_ptr<project_url>(new project_url{root});
}

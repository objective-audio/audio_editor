//
//  ae_project.cpp
//

#include "ae_project.h"

using namespace yas;
using namespace yas::ae;

project::project(url const &file_url) : _file_url(file_url) {
}

project_ptr project::make_shared(url const &file_url) {
    return std::shared_ptr<project>(new project{file_url});
}

url const &project::file_url() const {
    return this->_file_url;
}

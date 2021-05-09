//
//  ae_file_track.cpp
//

#include "ae_file_track.h"

using namespace yas;
using namespace yas::ae;

file_track::module_map_t const &file_track::modules() const {
    return this->_modules;
}

void file_track::insert_module(file_module const &module) {
    this->_modules.emplace(module.range, module);
}

std::shared_ptr<file_track> file_track::make_shared() {
    return std::shared_ptr<file_track>(new file_track{});
}

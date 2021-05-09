//
//  ae_file_track.cpp
//

#include "ae_file_track.h"

using namespace yas;
using namespace yas::ae;

file_track::file_track() {
    this->_event_fetcher = observing::fetcher<file_track_event>::make_shared(
        [this] { return file_track_event{.type = file_track_event_type::any, .modules = this->_modules}; });
}

file_track_module_map_t const &file_track::modules() const {
    return this->_modules;
}

void file_track::insert_module(file_module const &module) {
    this->_modules.emplace(module.range, module);
    this->_event_fetcher->push({.type = file_track_event_type::inserted, .module = module, .modules = this->_modules});
}

observing::syncable file_track::observe(std::function<void(file_track_event const &)> &&handler) {
    return this->_event_fetcher->observe(std::move(handler));
}

std::shared_ptr<file_track> file_track::make_shared() {
    return std::shared_ptr<file_track>(new file_track{});
}

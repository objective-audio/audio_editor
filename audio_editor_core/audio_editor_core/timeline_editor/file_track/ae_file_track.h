//
//  ae_file_track.h
//

#pragma once

#include <audio_editor_core/ae_file_module.h>
#include <audio_editor_core/ae_timeline_editor_dependency.h>

#include <map>

namespace yas::ae {
struct file_track final : timeline_file_track_interface {
    using module_map_t = std::map<proc::time::range, file_module>;
    module_map_t const &modules() const;

    void insert_module(file_module &&);

    static std::shared_ptr<file_track> make_shared();

   private:
    module_map_t _modules;

    file_track() = default;

    file_track(file_track const &) = delete;
    file_track(file_track &&) = delete;
    file_track &operator=(file_track const &) = delete;
    file_track &operator=(file_track &&) = delete;
};
}  // namespace yas::ae

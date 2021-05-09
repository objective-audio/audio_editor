//
//  ae_file_track.h
//

#pragma once

#include <audio_editor_core/ae_file_module.h>
#include <audio_editor_core/ae_timeline_editor_dependency.h>

#include <map>

namespace yas::ae {
struct file_track final : timeline_file_track_interface {
    static std::shared_ptr<file_track> make_shared();

   private:
    std::map<proc::time::range, file_module> _modules;

    file_track() = default;
};
}  // namespace yas::ae

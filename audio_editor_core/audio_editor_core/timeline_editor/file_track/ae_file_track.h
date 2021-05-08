//
//  ae_file_track.h
//

#pragma once

#include <audio_editor_core/ae_timeline_editor_dependency.h>

namespace yas::ae {
struct file_track final : timeline_file_track_interface {
    static std::shared_ptr<file_track> make_shared();

   private:
    file_track() = default;
};
}  // namespace yas::ae

//
//  ae_file_track.h
//

#pragma once

#include <audio_editor_core/ae_file_track_types.h>
#include <audio_editor_core/ae_timeline_editor_dependency.h>
#include <observing/yas_observing_umbrella.h>

namespace yas::ae {
struct file_track final : timeline_file_track_interface {
    file_track_module_map_t const &modules() const;

    void insert_module(file_module const &);

    observing::syncable observe(std::function<void(file_track_event const &)> &&);

    static std::shared_ptr<file_track> make_shared();

   private:
    file_track_module_map_t _modules;

    observing::fetcher_ptr<file_track_event> _event_fetcher;

    file_track();

    file_track(file_track const &) = delete;
    file_track(file_track &&) = delete;
    file_track &operator=(file_track const &) = delete;
    file_track &operator=(file_track &&) = delete;
};
}  // namespace yas::ae

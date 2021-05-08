//
//  ae_timeline_editor_dependency.h
//

#pragma once

#include <audio/yas_audio_umbrella.h>
#include <audio_editor_core/ae_file_loader_types.h>
#include <playing/yas_playing_umbrella.h>

#include <memory>

namespace yas::proc {
class timeline;
}

namespace yas::ae {
struct timeline_file_loader_interface {
    virtual ~timeline_file_loader_interface() = default;

    virtual std::optional<file_info> load_file_info(url const &) const = 0;
};

struct timeline_player_interface {
    virtual ~timeline_player_interface() = default;

    virtual void set_timeline(std::shared_ptr<proc::timeline> const &, playing::sample_rate_t const,
                              audio::pcm_format const) = 0;
    virtual void reset_timeline() = 0;
};

struct timeline_file_track_interface {
    virtual ~timeline_file_track_interface() = default;
};
}  // namespace yas::ae

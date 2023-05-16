//
//  ae_exporter_types.h
//

#pragma once

#include <audio/yas_audio_umbrella.hpp>
#include <playing/yas_playing_umbrella.hpp>

namespace yas::ae {
enum class exporting_error {
    remove_file_failed,
    create_file_failed,
    invalid_timeline,
    calc_frame_failed,
    copy_buffer_failed,
    write_file_failed,
};

using exporting_result = result<std::nullptr_t, exporting_error>;

struct exporting_format final {
    playing::sample_rate_t sample_rate;
    audio::pcm_format pcm_format;
    uint32_t channel_count;

    bool is_available() const {
        return this->sample_rate != 0 && this->pcm_format != audio::pcm_format::other && this->channel_count > 0;
    }
};
}  // namespace yas::ae

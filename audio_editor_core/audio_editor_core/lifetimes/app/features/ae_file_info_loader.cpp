//
//  ae_file_info_loader.cpp
//

#include "ae_file_info_loader.h"

#include <audio/yas_audio_umbrella.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<file_info_loader> file_info_loader::make_shared() {
    return std::make_shared<file_info_loader>();
}

file_info_loader::file_info_loader() {
}

std::optional<file_info> file_info_loader::load_file_info(url const &url) const {
    auto const file_result = audio::file::make_opened({.file_url = url});
    if (file_result.is_error()) {
        return std::nullopt;
    }

    auto const &file = file_result.value();

    auto const &processing_format = file->processing_format();

    return file_info{.sample_rate = static_cast<uint32_t>(std::round(processing_format.sample_rate())),
                     .channel_count = processing_format.channel_count(),
                     .length = static_cast<uint64_t>(file->processing_length())};
}

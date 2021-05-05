//
//  ae_file_loader.cpp
//

#include "ae_file_loader.h"

#include <audio/yas_audio_umbrella.h>

using namespace yas;
using namespace yas::ae;

file_loader::file_loader() {
}

std::optional<file_info> file_loader::load_file_info(url const &url) const {
    auto const file_result = audio::file::make_opened({.file_url = url});
    if (file_result.is_error()) {
        return std::nullopt;
    }

    auto const &file = file_result.value();

    auto const &processing_format = file->processing_format();

    return file_info{.sample_rate = processing_format.sample_rate(),
                     .channel_count = processing_format.channel_count(),
                     .length = static_cast<uint64_t>(file->processing_length())};
}

std::shared_ptr<file_loader> file_loader::make_shared() {
    return std::shared_ptr<file_loader>(new file_loader{});
}

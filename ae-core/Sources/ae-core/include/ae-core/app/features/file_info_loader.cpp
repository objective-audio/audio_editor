//
//  file_info_loader.cpp
//

#include "file_info_loader.h"

#include <audio-engine/umbrella.hpp>

using namespace yas;
using namespace yas::ae;

file_info_loader::file_info_loader() {
}

std::optional<file_info> file_info_loader::load_file_info(std::filesystem::path const &path) const {
    auto const file_result = audio::file::make_opened({.file_path = path});
    if (file_result.is_error()) {
        return std::nullopt;
    }

    auto const &file = file_result.value();

    auto const &processing_format = file->processing_format();

    return file_info{.sample_rate = static_cast<uint32_t>(std::round(processing_format.sample_rate())),
                     .channel_count = processing_format.channel_count(),
                     .length = static_cast<uint64_t>(file->processing_length())};
}

//
//  ae_file_importer.cpp
//

#include "ae_file_importer.h"

#include <audio/yas_audio_umbrella.h>
#include <cpp_utils/yas_thread.h>

using namespace yas;
using namespace yas::ae;

void file_importer::import(url const &src_url, url const &dst_url, std::function<void(bool const)> &&completion) {
    this->_queue.push_back(task::make_shared([src_url, dst_url, &completion](task const &task) {
        auto const src_file_result = audio::file::make_opened({.file_url = src_url});

        if (src_file_result.is_error()) {
            completion(false);
            return;
        }

        auto const src_file = src_file_result.value();
        auto const &src_file_format = src_file->file_format();
        auto const &proc_format = src_file->processing_format();

        auto const dst_settings =
            audio::wave_file_settings(src_file_format.sample_rate(), src_file_format.channel_count(), 32);

        auto const dst_file_result = audio::file::make_created(
            {.file_url = dst_url, .file_type = audio::file_type::core_audio_format, .settings = dst_settings});

        if (dst_file_result.is_error()) {
            src_file->close();
            completion(false);
            return;
        }

        auto const dst_file = dst_file_result.value();

        auto wrapped_completion = [src_file, dst_file, completion = std::move(completion)](bool const result) {
            src_file->close();
            dst_file->close();
            completion(result);
        };

        dst_file->set_processing_format(proc_format);

        uint32_t const max_copy_length = 1024;
        audio::pcm_buffer copying_buffer{proc_format, max_copy_length};

        while (true) {
            if (task.is_canceled()) {
                wrapped_completion(false);
                return;
            }

            copying_buffer.set_frame_length(max_copy_length);

            auto read_result = src_file->read_into_buffer(copying_buffer);
            if (read_result.is_error()) {
                wrapped_completion(false);
                return;
            }

            if (copying_buffer.frame_length() == 0) {
                break;
            }

            auto write_result = dst_file->write_from_buffer(copying_buffer);
            if (write_result.is_error()) {
                wrapped_completion(false);
                return;
            }
        }

        wrapped_completion(true);
    }));
}

std::shared_ptr<file_importer> file_importer::make_shared() {
    return std::shared_ptr<file_importer>(new file_importer{});
}

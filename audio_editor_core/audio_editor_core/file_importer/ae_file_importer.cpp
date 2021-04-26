//
//  ae_file_importer.cpp
//

#include "ae_file_importer.h"

#include <audio/yas_audio_umbrella.h>
#include <cpp_utils/yas_file_manager.h>
#include <cpp_utils/yas_thread.h>

using namespace yas;
using namespace yas::ae;

file_importer::file_importer(task_queue const &queue, task_priority_t const priority)
    : _queue(queue), _priority(priority) {
}

void file_importer::import(url const &src_url, url const &dst_url, std::function<void(bool const)> &&completion) {
    this->_queue.push_back(task::make_shared(
        [src_url, dst_url, completion = std::move(completion)](task const &task) {
            std::optional<audio::file_ptr> src_file{std::nullopt};
            std::optional<audio::file_ptr> dst_file{std::nullopt};

            auto wrapped_completion = [&src_file, &dst_file, completion = std::move(completion)](bool const result) {
                if (src_file.has_value()) {
                    src_file.value()->close();
                }
                if (dst_file.has_value()) {
                    dst_file.value()->close();
                }
                completion(result);
            };

            auto const src_file_result = audio::file::make_opened({.file_url = src_url});

            if (src_file_result.is_error()) {
                wrapped_completion(false);
                return;
            }

            src_file = src_file_result.value();
            auto const &src_file_format = src_file.value()->file_format();
            auto const &proc_format = src_file.value()->processing_format();

            auto const dst_settings =
                audio::wave_file_settings(src_file_format.sample_rate(), src_file_format.channel_count(), 32);

            auto const dst_directory = dst_url.deleting_last_path_component();
            if (auto result = file_manager::create_directory_if_not_exists(dst_directory.path()); result.is_error()) {
                wrapped_completion(false);
                return;
            }

            auto const dst_file_result = audio::file::make_created(
                {.file_url = dst_url, .file_type = audio::file_type::core_audio_format, .settings = dst_settings});

            if (dst_file_result.is_error()) {
                wrapped_completion(false);
                return;
            }

            dst_file = dst_file_result.value();

            dst_file.value()->set_processing_format(proc_format);

            uint32_t const max_copy_length = 1024;
            audio::pcm_buffer copying_buffer{proc_format, max_copy_length};

            while (true) {
                if (task.is_canceled()) {
                    wrapped_completion(false);
                    return;
                }

                copying_buffer.set_frame_length(max_copy_length);

                auto read_result = src_file.value()->read_into_buffer(copying_buffer);
                if (read_result.is_error()) {
                    wrapped_completion(false);
                    return;
                }

                if (copying_buffer.frame_length() == 0) {
                    break;
                }

                auto write_result = dst_file.value()->write_from_buffer(copying_buffer);
                if (write_result.is_error()) {
                    wrapped_completion(false);
                    return;
                }
            }

            wrapped_completion(true);
        },
        {.priority = this->_priority}));
}

std::shared_ptr<file_importer> file_importer::make_shared(task_queue const &queue, task_priority_t const priority) {
    return std::shared_ptr<file_importer>(new file_importer{queue, priority});
}

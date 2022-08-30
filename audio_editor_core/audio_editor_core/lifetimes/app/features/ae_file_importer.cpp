//
//  ae_file_importer.cpp
//

#include "ae_file_importer.h"

#include <audio/yas_audio_umbrella.h>
#include <audio_editor_core/ae_file_importer_resource.h>
#include <cpp_utils/yas_file_manager.h>
#include <cpp_utils/yas_thread.h>

#include <thread>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<file_importer> file_importer::make_shared(workable_ptr const &worker, uint32_t const priority) {
    return std::make_shared<file_importer>(worker, priority);
}

file_importer::file_importer(workable_ptr const &worker, uint32_t const priority)
    : _worker(worker), _priority(priority), _resource(file_importer_resource::make_shared()) {
    this->_worker->add_task(this->_priority, [resource = this->_resource] {
        auto context_opt = resource->pull_context_on_task();
        if (!context_opt.has_value()) {
            return worker_task_result::unprocessed;
        }
        auto &context = context_opt.value();

        std::optional<audio::file_ptr> src_file{std::nullopt};
        std::optional<audio::file_ptr> dst_file{std::nullopt};

        auto wrapped_completion = [&src_file, &dst_file, &context](bool const result) {
            if (src_file.has_value()) {
                src_file.value()->close();
            }

            if (dst_file.has_value()) {
                dst_file.value()->close();
            }

            thread::perform_async_on_main([result, completion = std::move(context.completion)] { completion(result); });
        };

        auto const src_file_result = audio::file::make_opened({.file_path = context.src_path});

        if (src_file_result.is_error()) {
            wrapped_completion(false);
            return worker_task_result::processed;
        }

        src_file = src_file_result.value();
        auto const &src_proc_format = src_file.value()->processing_format();

        audio::format const proc_format{
            audio::format::args{.sample_rate = static_cast<double>(context.project_format.sample_rate),
                                .channel_count = context.project_format.channel_count,
                                .pcm_format = src_proc_format.pcm_format(),
                                .interleaved = src_proc_format.is_interleaved()}};

        src_file.value()->set_processing_format(proc_format);

        auto const dst_settings =
            audio::wave_file_settings(context.project_format.sample_rate, context.project_format.channel_count, 32);

        auto dst_path = context.dst_path;
        auto const dst_directory = dst_path.parent_path();
        if (auto result = file_manager::create_directory_if_not_exists(dst_directory); result.is_error()) {
            wrapped_completion(false);
            return worker_task_result::processed;
        }

        auto const dst_file_result = audio::file::make_created({.file_path = context.dst_path,
                                                                .file_type = audio::file_type::core_audio_format,
                                                                .settings = dst_settings});

        if (dst_file_result.is_error()) {
            wrapped_completion(false);
            return worker_task_result::processed;
        }

        dst_file = dst_file_result.value();

        dst_file.value()->set_processing_format(proc_format);

        uint32_t const max_copy_length = 4096;
        audio::pcm_buffer copying_buffer{proc_format, max_copy_length};

        while (true) {
            std::this_thread::yield();

            auto const cancel_ids = resource->pull_cancel_ids();
            if (contains(cancel_ids, context.project_id.raw_value)) {
                wrapped_completion(false);
                return worker_task_result::processed;
            }

            copying_buffer.set_frame_length(max_copy_length);

            auto read_result = src_file.value()->read_into_buffer(copying_buffer);
            if (read_result.is_error()) {
                wrapped_completion(false);
                return worker_task_result::processed;
            }

            if (copying_buffer.frame_length() == 0) {
                break;
            }

            std::this_thread::yield();

            auto write_result = dst_file.value()->write_from_buffer(copying_buffer);
            if (write_result.is_error()) {
                wrapped_completion(false);
                return worker_task_result::processed;
            }
        }

        wrapped_completion(true);

        return worker_task_result::processed;
    });
}

void file_importer::import(file_importing_context &&context) {
    this->_resource->push_context_on_main(std::move(context));
}

void file_importer::cancel(project_id const &cancel_id) {
    this->_resource->cancel_on_main(cancel_id);
}

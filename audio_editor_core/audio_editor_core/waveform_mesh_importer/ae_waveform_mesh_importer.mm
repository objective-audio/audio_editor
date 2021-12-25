//
//  ae_waveform_mesh_importer.cpp
//

#include "ae_waveform_mesh_importer.h"
#include <Accelerate/Accelerate.h>
#include <audio/yas_audio_file.h>
#include <audio/yas_audio_pcm_buffer.h>
#include <audio_editor_core/ae_ui_track_constants.h>
#include <cpp_utils/yas_thread.h>
#include <ui/yas_ui_umbrella.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<waveform_mesh_importer> waveform_mesh_importer::make_shared(url const &url) {
    auto shared = std::shared_ptr<waveform_mesh_importer>(new waveform_mesh_importer{url});
    shared->_weak_importer = shared;
    return shared;
}

waveform_mesh_importer::waveform_mesh_importer(url const &url)
    : _url(url),
      _notifier(observing::notifier<event>::make_shared()),
      _task_queue(task_queue<identifier>::make_shared()) {
}

void waveform_mesh_importer::import(std::size_t const idx, module_location const &location, float const width_per_sec) {
    this->_task_queue->cancel([&location](auto const &identifier) { return location.identifier == identifier; });

    auto const task = yas::task<identifier>::make_shared(
        [idx, location, width_per_sec, url = this->_url,
         weak_importer = this->_weak_importer](yas::task<identifier> const &task) {
            if (weak_importer.expired()) {
                return;
            }

            std::shared_ptr<ui::mesh_vertex_data> vertex_data = nullptr;
            std::shared_ptr<ui::mesh_index_data> index_data = nullptr;

            auto const file_result = audio::file::make_opened({.file_url = url});
            if (file_result) {
                double const mesh_width = location.width * width_per_sec;
                uint32_t const floored_mesh_width = static_cast<uint32_t>(std::floor(mesh_width));
                uint32_t const ceiled_mesh_width = static_cast<uint32_t>(std::ceil(mesh_width));
                bool const has_fraction = floored_mesh_width != ceiled_mesh_width;
                uint32_t const rect_count = floored_mesh_width + (has_fraction ? 1 : 0);
                uint32_t const vertex_count = rect_count * ui::vertex2d_rect::vector_count;
                uint32_t const index_count = rect_count * ui::index2d_rect::vector_count;

                auto const mesh_vertex_data = ui::dynamic_mesh_vertex_data::make_shared(vertex_count);
                auto const mesh_index_data = ui::dynamic_mesh_index_data::make_shared(index_count);
                vertex_data = mesh_vertex_data;
                index_data = mesh_index_data;

                auto const &file = file_result.value();
                auto const &format = file->processing_format();
                auto const frames_per_sec = static_cast<uint32_t>(format.sample_rate());
                file->set_file_frame_position(static_cast<uint32_t>(location.range.frame));

                audio::pcm_buffer buffer{format, frames_per_sec};

                float max = 0.0f;
                float min = 0.0f;
                uint32_t file_head_frame = 0;
                uint32_t next_file_frame =
                    static_cast<uint32_t>(static_cast<double>(location.range.length) / mesh_width);

                uint32_t vertex_rect_idx = 0;

                auto const set_data = [&mesh_vertex_data, &mesh_width, &floored_mesh_width](
                                          uint32_t const vertex_rect_idx, float const max, float const min) {
                    mesh_vertex_data->write([&max, &min, &vertex_rect_idx, &mesh_width,
                                             &floored_mesh_width](std::vector<ui::vertex2d_t> &vec) {
                        auto *vertex_rects_data = (ui::vertex2d_rect *)vec.data();
                        float const width =
                            (floored_mesh_width == vertex_rect_idx) ? (mesh_width - floored_mesh_width) : 1.0f;
                        vertex_rects_data[vertex_rect_idx].set_position(
                            ui::region{.origin = {.x = static_cast<float>(vertex_rect_idx), .y = min},
                                       .size = {.width = width, .height = max - min}});
                    });
                };

                bool is_cancelled = false;

                while (file_head_frame < location.range.length) {
                    if (task.is_canceled()) {
                        is_cancelled = true;
                        break;
                    }

                    if (auto result = file->read_into_buffer(buffer); !result) {
                        is_cancelled = true;
                        break;
                    }

                    auto const *const data = buffer.data_ptr_at_index<float>(0);

                    uint32_t buffer_head_frame = 0;
                    auto const &buffer_frame_length = buffer.frame_length();

                    while (buffer_head_frame < buffer_frame_length) {
                        auto const process_length = std::min((buffer_frame_length - buffer_head_frame),
                                                             (next_file_frame - buffer_head_frame - file_head_frame));

                        float local_max = 0.0f;
                        float local_min = 0.0f;
                        vDSP_maxv(&data[buffer_head_frame], 1, &local_max, process_length);
                        vDSP_minv(&data[buffer_head_frame], 1, &local_min, process_length);
                        max = std::max(max, std::max(local_max, 0.0f));
                        min = std::min(min, std::min(local_min, 0.0f));

                        buffer_head_frame += process_length;

                        if ((file_head_frame + buffer_head_frame) == next_file_frame) {
                            if (vertex_rect_idx < rect_count) {
                                set_data(vertex_rect_idx, max, min);
                            } else {
                                is_cancelled = true;
                                break;
                            }

                            vertex_rect_idx += 1;
                            next_file_frame = static_cast<uint32_t>(
                                static_cast<double>((vertex_rect_idx + 1) * location.range.length) / mesh_width);
                            max = min = 0.0f;
                        }
                    }

                    if (is_cancelled) {
                        break;
                    }

                    file_head_frame += buffer.frame_length();
                }

                if (vertex_rect_idx < rect_count) {
                    set_data(vertex_rect_idx, max, min);
                }

                file->close();

                mesh_index_data->write([](std::vector<ui::index2d_t> &vector) {
                    auto *const index_rects_data = (ui::index2d_rect *)vector.data();
                    std::size_t const index_rects_size = vector.size() / ui::index2d_rect::vector_count;
                    auto each = make_fast_each(index_rects_size);
                    while (yas_each_next(each)) {
                        auto const &rect_idx = yas_each_index(each);
                        uint32_t const rect_head_idx =
                            static_cast<uint32_t>(rect_idx * ui::vertex2d_rect::vector_count);
                        index_rects_data[rect_idx].set_all(rect_head_idx);
                    }
                });
            } else {
                auto const mesh_vertex_data = ui::static_mesh_vertex_data::make_shared(2);
                auto const mesh_index_data = ui::static_mesh_index_data::make_shared(2);
                vertex_data = mesh_vertex_data;
                index_data = mesh_index_data;

                mesh_vertex_data->write_once([&location](std::vector<ui::vertex2d_t> &vector) {
                    vector[0].position = {0.0f, 0.0f};
                    vector[1].position = {location.width, 0.0f};
                });

                mesh_index_data->write_once([](std::vector<ui::index2d_t> &vector) {
                    vector[0] = 0;
                    vector[1] = 1;
                });
            }

            if (task.is_canceled()) {
                return;
            }

            thread::perform_async_on_main([weak_importer, idx, identifier = location.identifier,
                                           vertex_data = std::move(vertex_data), index_data = std::move(index_data)] {
                if (auto const importer = weak_importer.lock()) {
                    importer->_notifier->notify(
                        {.index = idx,
                         .identifier = identifier,
                         .datas = {{.vertex_data = std::move(vertex_data), .index_data = std::move(index_data)}}});
                }
            });
        },
        {.canceller = location.identifier});

    this->_task_queue->push_back(task);
}

void waveform_mesh_importer::cancel(identifier const &cancel_id) {
    this->_task_queue->cancel([&cancel_id](auto const &identifier) { return cancel_id == identifier; });
}

void waveform_mesh_importer::cancel_all() {
    this->_task_queue->cancel_all();
}

observing::endable waveform_mesh_importer::observe(std::function<void(event const &)> &&handler) {
    return this->_notifier->observe(std::move(handler));
}

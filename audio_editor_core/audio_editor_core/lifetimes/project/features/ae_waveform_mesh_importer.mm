//
//  ae_waveform_mesh_importer.cpp
//

#include "ae_waveform_mesh_importer.h"
#include <Accelerate/Accelerate.h>
#include <audio/yas_audio_file.h>
#include <audio/yas_audio_pcm_buffer.h>
#include <audio_editor_core/ae_hierarchy.h>
#include <audio_editor_core/ae_module_pool.h>
#include <audio_editor_core/ae_project_path.h>
#include <audio_editor_core/ae_ui_zooming_constants.h>
#include <cpp_utils/yas_thread.h>
#include <ui/yas_ui_umbrella.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<waveform_mesh_importer> waveform_mesh_importer::make_shared(window_lifetime_id const &lifetime_id,
                                                                            module_pool *module_pool) {
    auto const &window_lifetime = hierarchy::window_lifetime_for_id(lifetime_id);

    return std::make_shared<waveform_mesh_importer>(window_lifetime->project_path.get(), module_pool);
}

waveform_mesh_importer::waveform_mesh_importer(project_path const *project_path, module_pool *module_pool)
    : _project_path(project_path),
      _module_pool(module_pool),
      _notifier(observing::notifier<event>::make_shared()),
      _task_queue(task_queue<object_id>::make_shared()) {
}

void waveform_mesh_importer::import(std::size_t const idx, module_content const &content) {
    this->_task_queue->cancel([&content](auto const &identifier) { return content.identifier == identifier; });

    auto const &modules = this->_module_pool->modules();
    if (!modules.contains(content.index())) {
        return;
    }

    auto path = this->_project_path->editing_files_directory().append(modules.at(content.index()).value.file_name);
    auto const file_frame = modules.at(content.index()).value.file_frame;

    auto const task = yas::task<object_id>::make_shared(
        [idx, content, path = std::move(path), file_frame,
         weak_importer = this->weak_from_this()](yas::task<object_id> const &task) {
            if (weak_importer.expired()) {
                return;
            }

            std::vector<waveform_mesh_importer_event::data> datas;

            auto const file_result = audio::file::make_opened({.file_path = path});
            if (file_result) {
                auto const &scale = content.scale;
                double const rect_width = 1.0 / scale;

                struct dynamic_data final {
                    std::shared_ptr<ui::dynamic_mesh_vertex_data> vertex_data;
                    std::shared_ptr<ui::dynamic_mesh_index_data> index_data;
                };

                auto const &file = file_result.value();
                auto const &format = file->processing_format();
                auto const frames_per_sec = static_cast<uint32_t>(format.sample_rate());

                audio::pcm_buffer buffer{format, frames_per_sec};

                auto each = make_fast_each(content.mesh_elements.size());
                while (yas_each_next(each)) {
                    if (task.is_canceled()) {
                        break;
                    }

                    auto const &data_idx = yas_each_index(each);
                    auto const &mesh_element = content.mesh_elements.at(data_idx);

                    if (!mesh_element.has_value()) {
                        datas.emplace_back(waveform_mesh_importer_event::data{nullptr, nullptr});
                        continue;
                    }

                    auto const rect_count = content.mesh_elements.at(data_idx).value().rect_count;

                    if (rect_count == 0) {
                        datas.emplace_back(waveform_mesh_importer_event::data{nullptr, nullptr});
                        continue;
                    }

                    uint32_t const vertex_count = rect_count * ui::vertex2d_rect::vector_count;
                    uint32_t const index_count = rect_count * ui::index2d_rect::vector_count;

                    auto const mesh_vertex_data = ui::dynamic_mesh_vertex_data::make_shared(vertex_count);
                    auto const mesh_index_data = ui::dynamic_mesh_index_data::make_shared(index_count);

                    mesh_index_data->write([](std::vector<ui::index2d_t> &vector) {
                        if (vector.size() == 0) {
                            return;
                        }

                        auto *const index_rects_data = (ui::index2d_rect *)vector.data();
                        std::size_t const index_rects_size = vector.size() / ui::index2d_rect::vector_count;
                        auto local_each = make_fast_each(index_rects_size);
                        while (yas_each_next(local_each)) {
                            auto const &rect_idx = yas_each_index(local_each);
                            uint32_t const rect_head_idx =
                                static_cast<uint32_t>(rect_idx * ui::vertex2d_rect::vector_count);
                            index_rects_data[rect_idx].set_all(rect_head_idx);
                        }
                    });

                    dynamic_data dynamic_data{mesh_vertex_data, mesh_index_data};
                    datas.emplace_back(waveform_mesh_importer_event::data{mesh_vertex_data, mesh_index_data});

                    auto const &mesh_element_value = mesh_element.value();

                    auto const mesh_element_offset_x = content.element_offset_x(data_idx);
                    if (!mesh_element_offset_x.has_value()) {
                        continue;
                    }

                    auto const &mesh_element_head_x_value = mesh_element_offset_x.value();

                    file->set_file_frame_position(static_cast<uint32_t>(file_frame + mesh_element_value.range.frame));

                    float max = 0.0f;
                    float min = 0.0f;

                    uint32_t next_file_frame =
                        static_cast<uint32_t>(static_cast<double>(mesh_element_value.range.length) /
                                              static_cast<double>(mesh_element_value.rect_count));

                    uint32_t vertex_rect_idx = 0;

                    auto const write_handler = [&max, &min, &vertex_rect_idx, &rect_width,
                                                &mesh_element_head_x_value](std::vector<ui::vertex2d_t> &vec) {
                        auto *vertex_rects_data = (ui::vertex2d_rect *)vec.data();
                        // moduleの終わりのはみ出る部分を調整したい
                        if ((vertex_rect_idx * ui::vertex2d_rect::vector_count) < vec.size()) {
                            vertex_rects_data[vertex_rect_idx].set_position(
                                ui::region{.origin = {.x = mesh_element_head_x_value +
                                                           static_cast<float>(vertex_rect_idx * rect_width),
                                                      .y = min},
                                           .size = {.width = static_cast<float>(rect_width), .height = max - min}});
                        } else {
                            assert(0);
                        }
                    };

                    uint32_t file_head_frame = 0;
                    while (file_head_frame < content.range.length) {
                        if (task.is_canceled()) {
                            break;
                        }

                        if (auto result = file->read_into_buffer(buffer); !result) {
                            break;
                        }

                        auto const &buffer_frame_length = buffer.frame_length();
                        if (buffer_frame_length == 0) {
                            break;
                        }

                        auto const *const data = buffer.data_ptr_at_index<float>(0);

                        bool is_cancelled = false;

                        uint32_t buffer_head_frame = 0;
                        while (buffer_head_frame < buffer_frame_length) {
                            auto const process_length =
                                std::min((buffer_frame_length - buffer_head_frame),
                                         (next_file_frame - buffer_head_frame - file_head_frame));

                            float local_max = 0.0f;
                            float local_min = 0.0f;
                            vDSP_maxv(&data[buffer_head_frame], 1, &local_max, process_length);
                            vDSP_minv(&data[buffer_head_frame], 1, &local_min, process_length);
                            max = std::max(max, std::max(local_max, 0.0f));
                            min = std::min(min, std::min(local_min, 0.0f));

                            buffer_head_frame += process_length;

                            if ((file_head_frame + buffer_head_frame) == next_file_frame) {
                                if (vertex_rect_idx < mesh_element_value.rect_count) {
                                    dynamic_data.vertex_data->write(write_handler);
                                } else {
                                    is_cancelled = true;
                                    break;
                                }

                                vertex_rect_idx += 1;
                                next_file_frame = static_cast<uint32_t>(
                                    static_cast<double>((vertex_rect_idx + 1) * mesh_element_value.range.length) /
                                    static_cast<double>(mesh_element_value.rect_count));
                                max = min = 0.0f;
                            }
                        }

                        if (is_cancelled) {
                            break;
                        }

                        file_head_frame += buffer.frame_length();
                    }
                }

                file->close();
            } else {
                auto const mesh_vertex_data = ui::dynamic_mesh_vertex_data::make_shared(2);
                auto const mesh_index_data = ui::static_mesh_index_data::make_shared(2);
                datas.emplace_back(waveform_mesh_importer_event::data{mesh_vertex_data, mesh_index_data});

                mesh_vertex_data->write([&content](std::vector<ui::vertex2d_t> &vector) {
                    vector[0].position = {0.0f, 0.0f};
                    vector[1].position = {content.width(), 0.0f};
                });

                mesh_index_data->write_once([](std::vector<ui::index2d_t> &vector) {
                    vector[0] = 0;
                    vector[1] = 1;
                });
            }

            if (task.is_canceled()) {
                return;
            }

            thread::perform_async_on_main([weak_importer, idx, identifier = content.identifier,
                                           datas = std::move(datas)] {
                if (auto const importer = weak_importer.lock()) {
                    importer->_notifier->notify({.index = idx, .identifier = identifier, .datas = std::move(datas)});
                }
            });
        },
        {.canceller = content.identifier});

    this->_task_queue->push_back(task);
}

void waveform_mesh_importer::cancel(object_id const &cancel_id) {
    this->_task_queue->cancel([&cancel_id](auto const &identifier) { return cancel_id == identifier; });
}

void waveform_mesh_importer::cancel_all() {
    this->_task_queue->cancel_all();
}

observing::endable waveform_mesh_importer::observe(std::function<void(event const &)> &&handler) {
    return this->_notifier->observe(std::move(handler));
}

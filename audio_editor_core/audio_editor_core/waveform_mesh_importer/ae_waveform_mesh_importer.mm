//
//  ae_waveform_mesh_importer.cpp
//

#include "ae_waveform_mesh_importer.h"

#include <cpp_utils/yas_thread.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<waveform_mesh_importer> waveform_mesh_importer::make_shared() {
    auto shared = std::shared_ptr<waveform_mesh_importer>(new waveform_mesh_importer{});
    shared->_weak_importer = shared;
    return shared;
}

waveform_mesh_importer::waveform_mesh_importer()
    : _notifier(observing::notifier<event>::make_shared()), _task_queue(task_queue<identifier>::make_shared()) {
}

void waveform_mesh_importer::import(std::size_t const idx, module_location const &location) {
    this->_task_queue->cancel([&location](auto const &identifier) { return location.identifier == identifier; });

    auto const task = yas::task<identifier>::make_shared(
        [idx, location, weak_importer = this->_weak_importer](yas::task<identifier> const &task) {
            if (weak_importer.expired()) {
                return;
            }

            auto vertex_data = ui::static_mesh_vertex_data::make_shared(2);
            auto index_data = ui::static_mesh_index_data::make_shared(2);

            vertex_data->write_once([&location](std::vector<ui::vertex2d_t> &vector) {
                vector[0].position = {0.0f, 0.0f};
                vector[1].position = {location.width, 0.0f};
            });

            index_data->write_once([](std::vector<ui::index2d_t> &vector) {
                vector[0] = 0;
                vector[1] = 1;
            });

            if (task.is_canceled()) {
                return;
            }

            thread::perform_async_on_main([weak_importer, idx, identifier = location.identifier,
                                           vertex_data = std::move(vertex_data), index_data = std::move(index_data)] {
                if (auto const importer = weak_importer.lock()) {
                    importer->_notifier->notify({.index = idx,
                                                 .identifier = identifier,
                                                 .mesh_vertex_data = std::move(vertex_data),
                                                 .mesh_index_data = std::move(index_data)});
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

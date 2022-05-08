//
//  ae_exporter.cpp
//

#include "ae_exporter.h"

#include <audio/yas_audio_file.h>
#include <cpp_utils/yas_file_manager.h>
#include <cpp_utils/yas_task_queue.h>
#include <cpp_utils/yas_thread.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<exporter> exporter::make_shared() {
    return std::shared_ptr<exporter>(new exporter{});
}

exporter::exporter()
    : _last_result(nullptr),
      _is_exporting(observing::value::holder<bool>::make_shared(false)),
      _queue(task_queue<std::nullptr_t>::make_shared()) {
}

void exporter::begin(url const &export_url, std::shared_ptr<proc::timeline> const &timeline,
                     exporting_format const &format, time::range const &range) {
    if (!format.is_available()) {
        throw std::invalid_argument("invalid format.");
    }

    if (this->_is_exporting->value()) {
        throw std::runtime_error("exporting.");
    }

    this->_last_result = exporting_result{nullptr};
    this->_is_exporting->set_value(true);

    this->_queue->push_back(
        task<std::nullptr_t>::make_shared([timeline = std::move(timeline), format, export_url, range,
                                           weak_exporter = this->weak_from_this()](auto const &) {
            assert(!thread::is_main());

            exporting_result result{nullptr};

            auto const path = export_url.path();

            if (file_manager::content_exists(path)) {
                if (!file_manager::remove_content(path)) {
                    result = exporting_result{exporting_error::remove_file_failed};
                }
            }

            std::shared_ptr<audio::file> file = nullptr;

            if (result) {
                auto const settings = audio::wave_file_settings(format.sample_rate, format.channel_count, 32);
                if (auto create_result = audio::file::make_created({.file_url = export_url, .settings = settings})) {
                    file = create_result.value();
                } else {
                    result = exporting_result{exporting_error::create_file_failed};
                }
            }

            if (result) {
                proc::length_t const slice_length = 512;
                audio::pcm_buffer buffer{file->processing_format(), slice_length};

                timeline->process(range, proc::sync_source{format.sample_rate, slice_length},
                                  [&file, &buffer, &result, ch_count = format.channel_count](
                                      time::range const &current_range, proc::stream const &stream) mutable {
                                      buffer.reset_buffer();
                                      buffer.set_frame_length(static_cast<uint32_t>(current_range.length));

                                      auto ch_each = make_fast_each(ch_count);
                                      while (yas_each_next(ch_each)) {
                                          auto const &ch_idx = yas_each_index(ch_each);
                                          if (!stream.channels().contains(ch_idx)) {
                                              continue;
                                          }

                                          auto const &channel = stream.channel(ch_idx);
                                          auto const &events = channel.filtered_events<float, proc::signal_event>();

                                          for (auto const &pair : events) {
                                              time::range const &range = pair.first;
                                              std::shared_ptr<proc::signal_event> const &signal = pair.second;
                                              float const *signal_data = signal->data<float>();

                                              auto const to_begin_frame = range.frame - current_range.frame;
                                              if (to_begin_frame < 0) {
                                                  result = exporting_result{exporting_error::calc_frame_failed};
                                                  return proc::continuation::abort;
                                              }

                                              auto const copy_result = buffer.copy_from(
                                                  signal_data, 1, 0, ch_idx, static_cast<uint32_t>(to_begin_frame),
                                                  static_cast<uint32_t>(range.length));
                                              if (!copy_result) {
                                                  result = exporting_result{exporting_error::copy_buffer_failed};
                                                  return proc::continuation::abort;
                                              }
                                          }
                                      }

                                      auto const write_result = file->write_from_buffer(buffer);
                                      if (!write_result) {
                                          result = exporting_result{exporting_error::write_file_failed};
                                          return proc::continuation::abort;
                                      }

                                      return proc::continuation::keep;
                                  });
            }

            if (file) {
                file->close();
            }

            thread::perform_async_on_main([weak_exporter, result] {
                assert(thread::is_main());

                if (auto const exporter = weak_exporter.lock()) {
                    exporter->_last_result = result;
                    exporter->_is_exporting->set_value(false);
                }
            });
        }));
}

bool exporter::is_exporting() const {
    return this->_is_exporting->value();
}

observing::syncable exporter::observe_is_exporting(std::function<void(bool const &)> &&handler) {
    return this->_is_exporting->observe(std::move(handler));
}

exporting_result exporter::last_result() const {
    return this->_last_result;
}

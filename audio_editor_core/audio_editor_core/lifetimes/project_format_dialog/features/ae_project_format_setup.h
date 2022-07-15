//
//  ae_project_format_setup.h
//

#pragma once

#include <audio_editor_core/ae_project_format.h>
#include <observing/yas_observing_umbrella.h>

#include <array>

namespace yas::ae {
struct project_format_setup {
    [[nodiscard]] static std::shared_ptr<project_format_setup> make_shared();

    project_format_setup();

    std::array<uint32_t, 4> const available_sample_rates;
    uint32_t const min_channel_count;
    uint32_t const max_channel_count;

    [[nodiscard]] std::size_t const &sample_rate_index() const;
    void set_sample_rate_index(std::size_t const);

    [[nodiscard]] uint32_t const &channel_count() const;
    void set_channel_count(uint32_t const);

    project_format format() const;

    [[nodiscard]] observing::syncable observe_sample_rate_index(std::function<void(std::size_t const &)> &&);
    [[nodiscard]] observing::syncable observe_channel_count(std::function<void(uint32_t const &)> &&);

   private:
    observing::value::holder_ptr<std::size_t> const _sample_rate_idx;
    observing::value::holder_ptr<uint32_t> const _channel_count;
};
}  // namespace yas::ae

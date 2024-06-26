//
//  nudging_test_utils.h
//

#pragma once

#include <ae-core/project_editing/features/nudging_dependencies.h>

namespace yas::ae::test_utils {
struct timing_stub final : timing_for_nudging {
    sample_rate_t sample_rate_value = 0;
    timing_fraction_kind fraction_kind_value = timing_fraction_kind::frame30;

    timing_stub(sample_rate_t const sample_rate, timing_fraction_kind const kind);

    sample_rate_t sample_rate() const override;

    timing_components components(frame_index_t const) const override;
    timing_components floored_components(timing_unit_kind const, frame_index_t const) const override;
    frame_index_t frame(timing_components const &) const override;
};

struct app_settings_stub final : app_settings_for_nudging {
    observing::value::holder_ptr<ae::timing_unit_kind> const kind_holder =
        observing::value::holder<ae::timing_unit_kind>::make_shared(ae::timing_unit_kind::fraction);

    void set_timing_unit_kind(ae::timing_unit_kind const kind) override {
        this->kind_holder->set_value(kind);
    }

    ae::timing_unit_kind timing_unit_kind() const override {
        return this->kind_holder->value();
    }

    observing::syncable observe_timing_unit_kind(std::function<void(ae::timing_unit_kind const &)> &&handler) override {
        return this->kind_holder->observe(std::move(handler));
    }
};
}  // namespace yas::ae::test_utils

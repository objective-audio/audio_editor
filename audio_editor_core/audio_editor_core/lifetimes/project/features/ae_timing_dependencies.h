//
//  ae_timing_dependencies.h
//

#pragma once

namespace yas::ae {
struct app_settings_for_timing {
    virtual ~app_settings_for_timing() = default;

    virtual void set_timing_fraction_kind(timing_fraction_kind const) = 0;
    [[nodiscard]] virtual timing_fraction_kind timing_fraction_kind() const = 0;
};
}  // namespace yas::ae

//
//  ae_window_presenter_dependency.h
//

#pragma once

#include <cpp_utils/yas_url.h>

#include <string>

namespace yas::ae {
struct project_for_window_presenter {
    virtual ~project_for_window_presenter() = default;

    [[nodiscard]] virtual bool can_close() const = 0;
    virtual void request_close() = 0;
};
}  // namespace yas::ae

//
//  ae_app_dependency.h
//

#pragma once

#include <cpp_utils/yas_url.h>
#include <observing/yas_observing_umbrella.h>

namespace yas::ae {
struct app_project_interface {
    enum class notification {
        should_close,
    };

    virtual ~app_project_interface() = default;

    virtual uintptr_t identifier() const = 0;
    virtual url const &file_url() const = 0;

    virtual bool can_close() const = 0;
    virtual void request_close() = 0;

    virtual observing::endable observe_notification(std::function<void(notification const &)> &&) = 0;
};
}  // namespace yas::ae

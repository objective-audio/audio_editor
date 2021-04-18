//
//  ae_project.h
//

#pragma once

#include <audio_editor_core/ae_ptr.h>
#include <cpp_utils/yas_url.h>
#include <observing/yas_observing_umbrella.h>

namespace yas::ae {
struct project {
    enum class notification {
        should_close,
    };

    uintptr_t identifier() const;

    url const &file_url() const;

    bool can_close() const;
    void request_close();

    observing::endable observe_notification(std::function<void(notification const &)> &&);

    static project_ptr make_shared(url const &);

   private:
    url const _file_url;
    observing::notifier_ptr<notification> const _notifier = observing::notifier<notification>::make_shared();

    project(url const &);

    project(project const &) = delete;
    project(project &&) = delete;
    project &operator=(project const &) = delete;
    project &operator=(project &&) = delete;
};
}  // namespace yas::ae

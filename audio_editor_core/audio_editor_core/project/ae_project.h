//
//  ae_project.h
//

#pragma once

#include <audio_editor_core/ae_app_dependency.h>
#include <audio_editor_core/ae_ptr.h>

namespace yas::ae {
struct project final : app_project_interface {
    [[nodiscard]] uintptr_t identifier() const override;
    [[nodiscard]] url const &file_url() const override;

    [[nodiscard]] bool can_close() const override;
    void request_close() override;

    [[nodiscard]] observing::endable observe_notification(std::function<void(notification const &)> &&) override;

    [[nodiscard]] static project_ptr make_shared(url const &);

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

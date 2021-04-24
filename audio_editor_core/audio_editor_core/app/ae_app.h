//
//  ae_app.h
//

#pragma once

#include <audio_editor_core/ae_app_dependency.h>
#include <audio_editor_core/ae_app_types.h>

namespace yas::ae {
struct app final {
    [[nodiscard]] static std::shared_ptr<app> make_shared();
    [[nodiscard]] static std::shared_ptr<app> global();

    [[nodiscard]] std::shared_ptr<project_pool> const &project_pool() const;
    [[nodiscard]] std::shared_ptr<system_url> const &system_url() const;

   private:
    std::shared_ptr<ae::project_pool> const _project_pool = project_pool::make_shared();
    std::shared_ptr<ae::system_url> const _system_url = system_url::make_shared();

    app();

    app(app const &) = delete;
    app(app &&) = delete;
    app &operator=(app const &) = delete;
    app &operator=(app &&) = delete;
};
}  // namespace yas::ae

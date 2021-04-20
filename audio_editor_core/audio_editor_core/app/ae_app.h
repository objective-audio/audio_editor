//
//  ae_app.h
//

#pragma once

#include <audio_editor_core/ae_app_dependency.h>
#include <audio_editor_core/ae_app_types.h>

namespace yas::ae {
struct app final {
    [[nodiscard]] static app_ptr make_shared();

    [[nodiscard]] project_pool_ptr const &project_pool() const;

   private:
    project_pool_ptr const _project_pool = project_pool::make_shared();

    app();

    app(app const &) = delete;
    app(app &&) = delete;
    app &operator=(app const &) = delete;
    app &operator=(app &&) = delete;
};
}  // namespace yas::ae

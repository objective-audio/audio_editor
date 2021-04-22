//
//  ae_app_presenter_dependency.h
//

#pragma once

#include <audio_editor_core/ae_project_pool_types.h>
#include <audio_editor_core/ae_ptr.h>
#include <cpp_utils/yas_url.h>
#include <observing/yas_observing_umbrella.h>

namespace yas::ae {
struct app_presenter_project_pool_interface {
    virtual ~app_presenter_project_pool_interface() = default;

    virtual project_ptr add_project(url const &file_url) = 0;
    [[nodiscard]] virtual observing::syncable observe_event(std::function<void(project_pool_event const &)> &&) = 0;
};
}  // namespace yas::ae

//
//  ae_app_presenter_dependency.h
//

#pragma once

#include <audio_editor_core/ae_project_level_collector_types.h>
#include <cpp_utils/yas_url.h>
#include <observing/yas_observing_umbrella.h>

namespace yas::ae {
struct project_pool_for_app_presenter {
    virtual ~project_pool_for_app_presenter() = default;

    virtual void add_level(url const &file_url) = 0;
    [[nodiscard]] virtual observing::syncable observe_event(
        std::function<void(project_level_collector_event const &)> &&) = 0;
};
}  // namespace yas::ae

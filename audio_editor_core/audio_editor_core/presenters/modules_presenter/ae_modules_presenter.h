//
//  ae_modules_presenter.h
//

#pragma once

#include <audio_editor_core/ae_module_location.h>
#include <audio_editor_core/ae_modules_presenter_dependency.h>
#include <observing/yas_observing_umbrella.h>

namespace yas::ae {
struct modules_presenter {
    [[nodiscard]] static std::shared_ptr<modules_presenter> make_shared(std::string const &project_id);
    [[nodiscard]] static std::shared_ptr<modules_presenter> make_shared(
        std::shared_ptr<project_editor_for_modules_presenter> const &);

    [[nodiscard]] std::vector<module_location> module_locations() const;
    [[nodiscard]] observing::syncable observe_modules(std::function<void(std::vector<module_location> const &)> &&);

   private:
    std::weak_ptr<project_editor_for_modules_presenter> _project_editor;

    modules_presenter(std::shared_ptr<project_editor_for_modules_presenter> const &);
};
}  // namespace yas::ae

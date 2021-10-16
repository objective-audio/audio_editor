//
//  ae_modules_presenter.h
//

#pragma once

#include <audio_editor_core/ae_module_location.h>
#include <observing/yas_observing_umbrella.h>

namespace yas::ae {
class project;

struct modules_presenter {
    [[nodiscard]] static std::shared_ptr<modules_presenter> make_shared(std::string const &project_id);
    [[nodiscard]] static std::shared_ptr<modules_presenter> make_shared(std::shared_ptr<project> const &);

    [[nodiscard]] std::vector<module_location> module_locations() const;
    [[nodiscard]] observing::syncable observe_modules(std::function<void(std::vector<module_location> const &)> &&);

   private:
    std::weak_ptr<project> _project;

    modules_presenter(std::shared_ptr<project> const &);
};
}  // namespace yas::ae

//
//  ae_project_presenter.h
//

#pragma once

#include <audio_editor_core/ae_project_lifetime_id.h>
#include <audio_editor_core/ae_project_presenter_dependency.h>
#include <audio_editor_core/ae_project_presenter_types.h>
#include <observing/yas_observing_umbrella.h>

namespace yas::ae {
class project_path;
class project_closer;
class project_sub_lifecycle;

struct project_presenter final {
    [[nodiscard]] static std::shared_ptr<project_presenter> make_shared(project_lifetime_id const &);

    project_presenter(ae::project_lifetime_id const &, std::shared_ptr<project_path> const &,
                      std::shared_ptr<project_closer> const &, std::shared_ptr<project_sub_lifecycle> const &);

    project_lifetime_id const lifetime_id;

    [[nodiscard]] std::string title() const;

    [[nodiscard]] bool should_close();

    [[nodiscard]] observing::syncable observe(std::function<void(project_presenter_event const &)> &&);

   private:
    std::weak_ptr<project_path> const _project_path;
    std::weak_ptr<project_closer> _closer;
    std::weak_ptr<project_sub_lifecycle> const _project_sub_lifecycle;

    project_presenter(project_presenter const &) = delete;
    project_presenter(project_presenter &&) = delete;
    project_presenter &operator=(project_presenter const &) = delete;
    project_presenter &operator=(project_presenter &&) = delete;
};
}  // namespace yas::ae

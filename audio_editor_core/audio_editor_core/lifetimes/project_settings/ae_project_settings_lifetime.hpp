//
//  ae_project_settings_lifetime.hpp
//

#pragma once

#include <audio_editor_core/ae_project_format.h>
#include <audio_editor_core/ae_project_lifetime_id.h>

namespace yas::ae {
class project_settings_closer;
class app_lifetime;
class project_lifetime;

struct project_settings_lifetime final {
    static std::shared_ptr<project_settings_lifetime> make_shared(project_lifetime_id const &);
    project_settings_lifetime(project_lifetime_id const &, app_lifetime const *, project_lifetime const *);

    project_lifetime_id const lifetime_id;

    project_format const project_format;
    std::shared_ptr<project_settings_closer> const closer;

   private:
    project_settings_lifetime(project_settings_lifetime const &) = delete;
    project_settings_lifetime(project_settings_lifetime &&) = delete;
    project_settings_lifetime &operator=(project_settings_lifetime const &) = delete;
    project_settings_lifetime &operator=(project_settings_lifetime &&) = delete;
};
}  // namespace yas::ae

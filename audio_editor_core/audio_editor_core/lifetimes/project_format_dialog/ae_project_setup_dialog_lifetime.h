//
//  ae_project_setup_dialog_lifetime.h
//

#pragma once

#include <audio_editor_core/ae_app_sub_lifetime_id.h>

#include <memory>

namespace yas::ae {
class project_format_setup;
class project_setup;

struct project_setup_dialog_lifetime final {
    [[nodiscard]] static std::shared_ptr<project_setup_dialog_lifetime> make_shared(
        project_setup_dialog_lifetime_id const &);

    project_setup_dialog_lifetime(project_setup_dialog_lifetime_id const &,
                                  std::shared_ptr<project_format_setup> const &,
                                  std::shared_ptr<project_setup> const &);

    project_setup_dialog_lifetime_id const lifetime_id;
    std::shared_ptr<project_format_setup> const format_setup;
    std::shared_ptr<project_setup> const setup;

   private:
    project_setup_dialog_lifetime(project_setup_dialog_lifetime const &) = delete;
    project_setup_dialog_lifetime(project_setup_dialog_lifetime &&) = delete;
    project_setup_dialog_lifetime &operator=(project_setup_dialog_lifetime const &) = delete;
    project_setup_dialog_lifetime &operator=(project_setup_dialog_lifetime &&) = delete;
};
}  // namespace yas::ae

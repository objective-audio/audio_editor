//
//  project_setup.h
//

#pragma once

#include <ae-core/global/value_types/app_sub_lifetime_id.h>

#include <filesystem>
#include <memory>

namespace yas::ae {
class project_format_setup;
class project_opener;
class app_modal_lifecycle;

struct project_setup final {
    [[nodiscard]] static std::shared_ptr<project_setup> make_shared(project_setup_dialog_lifetime_id const &,
                                                                    project_format_setup *);

    project_setup(project_setup_dialog_lifetime_id const &, project_format_setup *, project_opener *,
                  app_modal_lifecycle *);

    void select_directory(std::filesystem::path const &);
    void finalize();

   private:
    project_setup_dialog_lifetime_id const _lifetime_id;
    project_format_setup *const _format_setup;
    project_opener *_project_opener;
    app_modal_lifecycle *_lifecycle;
};
}  // namespace yas::ae

//
//  ae_project_setup.h
//

#pragma once

#include <cpp_utils/yas_url.h>

#include <memory>

namespace yas::ae {
class project_format_setup;
class project_preparer;
class app_dialog_sub_level_router;

struct project_setup final {
    [[nodiscard]] static std::shared_ptr<project_setup> make_shared(project_format_setup *);

    project_setup(project_format_setup *, std::shared_ptr<project_preparer> const &,
                  std::shared_ptr<app_dialog_sub_level_router> const &);

    void select_directory(url const &);
    void finalize();

   private:
    project_format_setup *const _format_setup;
    std::weak_ptr<project_preparer> _preparer;
    std::weak_ptr<app_dialog_sub_level_router> _router;
};
}  // namespace yas::ae

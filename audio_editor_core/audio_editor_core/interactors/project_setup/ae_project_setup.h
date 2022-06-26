//
//  ae_project_setup.h
//

#pragma once

#include <cpp_utils/yas_url.h>

#include <memory>

namespace yas::ae {
class project_format_setup;
class window_opener;
class app_dialog_lifecycle;

struct project_setup final {
    [[nodiscard]] static std::shared_ptr<project_setup> make_shared(project_format_setup *);

    project_setup(project_format_setup *, window_opener *, app_dialog_lifecycle *);

    void select_directory(url const &);
    void finalize();

   private:
    project_format_setup *const _format_setup;
    window_opener *_window_opener;
    app_dialog_lifecycle *_lifecycle;
};
}  // namespace yas::ae

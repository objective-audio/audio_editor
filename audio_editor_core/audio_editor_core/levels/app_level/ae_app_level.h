//
//  ae_app_level.h
//

#pragma once

#include <cpp_utils/yas_worker.h>

namespace yas::ae {
class app_launcher;
class project_level_router;
class system_url;
class file_importer;
class file_loader;
class color;
class ui_root_level_router;

struct app_level final {
    [[nodiscard]] static std::shared_ptr<app_level> make_shared();

    std::shared_ptr<worker> const worker;
    std::shared_ptr<ae::system_url> const system_url;
    std::shared_ptr<ae::app_launcher> const launcher;
    std::shared_ptr<ae::file_importer> const file_importer;
    std::shared_ptr<ae::file_loader> const file_loader;
    std::shared_ptr<ae::color> const color;

    std::shared_ptr<ae::project_level_router> const project_level_router;
    std::shared_ptr<ae::ui_root_level_router> const ui_root_level_router;

   private:
    app_level();

    app_level(app_level const &) = delete;
    app_level(app_level &&) = delete;
    app_level &operator=(app_level const &) = delete;
    app_level &operator=(app_level &&) = delete;
};
}  // namespace yas::ae

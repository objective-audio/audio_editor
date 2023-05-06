//
//  ae_app_lifetime.h
//

#pragma once

#include <cpp_utils/yas_worker.h>

namespace yas::ae {
class app_launcher;
class project_lifecycle;
class system_path;
class file_importer;
class file_info_loader;
class color;
class ui_resource_lifecycle;
class project_opener;
class app_modal_lifecycle;
class project_lifecycle;
class action_sender;
class uuid_generator;
class id_generator;
class app_settings;
class project_settings_lifecycle;
class app_settings_lifecycle;
class app_settings_opener;

struct app_lifetime final {
    [[nodiscard]] static std::shared_ptr<app_lifetime> make_shared();

    app_lifetime(std::shared_ptr<worker> const &, std::shared_ptr<system_path> const &,
                 std::shared_ptr<app_launcher> const &, std::shared_ptr<file_importer> const &,
                 std::shared_ptr<file_info_loader> const &, std::shared_ptr<color> const &,
                 std::shared_ptr<uuid_generator> const &, std::shared_ptr<id_generator> const &,
                 std::shared_ptr<app_settings> const &, std::shared_ptr<project_lifecycle> const &,
                 std::shared_ptr<project_settings_lifecycle> const &, std::shared_ptr<app_settings_lifecycle> const &,
                 std::shared_ptr<app_modal_lifecycle> const &, std::shared_ptr<ui_resource_lifecycle> const &,
                 std::shared_ptr<project_opener> const &, std::shared_ptr<app_settings_opener> const &,
                 std::shared_ptr<action_sender> const &);

    std::shared_ptr<worker> const worker;
    std::shared_ptr<system_path> const system_path;
    std::shared_ptr<app_launcher> const launcher;
    std::shared_ptr<file_importer> const file_importer;
    std::shared_ptr<file_info_loader> const file_info_loader;
    std::shared_ptr<color> const color;
    std::shared_ptr<uuid_generator> const uuid_generator;
    std::shared_ptr<id_generator> const id_generator;
    std::shared_ptr<app_settings> const app_settings;

    std::shared_ptr<project_lifecycle> const project_lifecycle;
    std::shared_ptr<project_settings_lifecycle> const project_settings_lifecycle;
    std::shared_ptr<app_settings_lifecycle> const app_settings_lifecycle;
    std::shared_ptr<app_modal_lifecycle> const modal_lifecycle;
    std::shared_ptr<ui_resource_lifecycle> const ui_resource_lifecycle;

    std::shared_ptr<project_opener> const project_opener;
    std::shared_ptr<app_settings_opener> const app_settings_opener;

    std::shared_ptr<action_sender> const action_sender;

   private:
    app_lifetime(app_lifetime const &) = delete;
    app_lifetime(app_lifetime &&) = delete;
    app_lifetime &operator=(app_lifetime const &) = delete;
    app_lifetime &operator=(app_lifetime &&) = delete;
};
}  // namespace yas::ae

//
//  ae_app_lifetime.cpp
//

#include "ae_app_lifetime.h"

#include <audio_editor_core/ae_action_sender.h>
#include <audio_editor_core/ae_app_launcher.h>
#include <audio_editor_core/ae_app_lifetime_types.h>
#include <audio_editor_core/ae_app_modal_lifecycle.h>
#include <audio_editor_core/ae_color.h>
#include <audio_editor_core/ae_file_importer.h>
#include <audio_editor_core/ae_file_info_loader.h>
#include <audio_editor_core/ae_hierarchy.h>
#include <audio_editor_core/ae_id_generator.h>
#include <audio_editor_core/ae_project_lifecycle.h>
#include <audio_editor_core/ae_project_opener.h>
#include <audio_editor_core/ae_system_path.h>
#include <audio_editor_core/ae_uuid_generator.h>

#include <audio_editor_core/ae_app_settings.hpp>
#include <audio_editor_core/ae_app_settings_lifecycle.hpp>
#include <audio_editor_core/ae_app_settings_opener.hpp>
#include <audio_editor_core/ae_project_settings_lifecycle.hpp>
#include <audio_editor_core/ae_project_setup_dialog_opener.hpp>
#include <audio_editor_core/ae_ui_resource_lifecycle.hpp>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<app_lifetime> app_lifetime::make_shared() {
    auto const worker = worker::make_shared();
    auto const system_path = std::make_shared<ae::system_path>();
    auto const file_info_loader = std::make_shared<ae::file_info_loader>();
    auto const action_sender = std::make_shared<ae::action_sender>(hierarchy::app_lifecycle().get());
    auto const id_generator = std::make_shared<ae::id_generator>();
    auto const uuid_generator = std::make_shared<ae::uuid_generator>();
    auto const app_settings = std::make_shared<ae::app_settings>();
    auto const project_lifecycle = std::make_shared<ae::project_lifecycle>(id_generator.get(), uuid_generator.get());
    auto const project_settings_lifecycle = std::make_shared<ae::project_settings_lifecycle>();
    auto const app_settings_lifecycle = std::make_shared<ae::app_settings_lifecycle>();
    auto const app_modal_lifecycle = std::make_shared<ae::app_modal_lifecycle>(id_generator.get());

    return std::make_shared<app_lifetime>(
        worker, system_path, std::make_shared<app_launcher>(worker, system_path),
        std::make_shared<ae::file_importer>(worker, static_cast<uint32_t>(worker_priority::file_importing)),
        file_info_loader, std::make_shared<ae::color>(), uuid_generator, id_generator, app_settings, project_lifecycle,
        project_settings_lifecycle, app_settings_lifecycle, app_modal_lifecycle,
        std::make_shared<ae::ui_resource_lifecycle>(),
        std::make_shared<ae::project_opener>(file_info_loader.get(), project_lifecycle.get()),
        std::make_shared<ae::app_settings_opener>(app_settings_lifecycle.get()),
        std::make_shared<ae::project_setup_dialog_opener>(app_modal_lifecycle.get()), action_sender);
}

app_lifetime::app_lifetime(
    std::shared_ptr<yas::worker> const &worker, std::shared_ptr<ae::system_path> const &system_path,
    std::shared_ptr<ae::app_launcher> const &launcher, std::shared_ptr<ae::file_importer> const &file_importer,
    std::shared_ptr<ae::file_info_loader> const &file_info_loader, std::shared_ptr<ae::color> const &color,
    std::shared_ptr<ae::uuid_generator> const &uuid_generator, std::shared_ptr<ae::id_generator> const &id_generator,
    std::shared_ptr<ae::app_settings> const &app_settings,
    std::shared_ptr<ae::project_lifecycle> const &project_lifecycle,
    std::shared_ptr<ae::project_settings_lifecycle> const &project_settings_lifecycle,
    std::shared_ptr<ae::app_settings_lifecycle> const &app_settings_lifecycle,
    std::shared_ptr<app_modal_lifecycle> const &modal_lifecycle,
    std::shared_ptr<ae::ui_resource_lifecycle> const &ui_resource_lifecycle,
    std::shared_ptr<ae::project_opener> const &project_opener,
    std::shared_ptr<ae::app_settings_opener> const &app_settings_opener,
    std::shared_ptr<ae::project_setup_dialog_opener> const &project_setup_dialog_opener,
    std::shared_ptr<ae::action_sender> const &action_sender)
    : worker(worker),
      system_path(system_path),
      launcher(launcher),
      file_importer(file_importer),
      file_info_loader(file_info_loader),
      color(color),
      uuid_generator(uuid_generator),
      id_generator(id_generator),
      app_settings(app_settings),
      project_lifecycle(project_lifecycle),
      project_settings_lifecycle(project_settings_lifecycle),
      app_settings_lifecycle(app_settings_lifecycle),
      modal_lifecycle(modal_lifecycle),
      ui_resource_lifecycle(ui_resource_lifecycle),
      project_opener(project_opener),
      app_settings_opener(app_settings_opener),
      project_setup_dialog_opener(project_setup_dialog_opener),
      action_sender(action_sender) {
}

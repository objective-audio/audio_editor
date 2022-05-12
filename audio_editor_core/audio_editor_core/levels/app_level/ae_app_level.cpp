//
//  ae_app_level.cpp
//

#include "ae_app_level.h"

#include <audio_editor_core/ae_app_launcher.h>
#include <audio_editor_core/ae_app_level_types.h>
#include <audio_editor_core/ae_color.h>
#include <audio_editor_core/ae_file_importer.h>
#include <audio_editor_core/ae_file_loader.h>
#include <audio_editor_core/ae_project_level_router.h>
#include <audio_editor_core/ae_system_url.h>
#include <audio_editor_core/ae_ui_root_level_router.h>

using namespace yas;
using namespace yas::ae;

app_level::app_level(std::shared_ptr<yas::worker> const &worker, std::shared_ptr<ae::system_url> const &system_url,
                     std::shared_ptr<ae::app_launcher> const &launcher,
                     std::shared_ptr<ae::file_importer> const &file_importer,
                     std::shared_ptr<ae::file_loader> const &file_loader, std::shared_ptr<ae::color> const &color,
                     std::shared_ptr<ae::project_level_router> const &project_level_router,
                     std::shared_ptr<ae::ui_root_level_router> const &ui_root_level_router)
    : worker(worker),
      system_url(system_url),
      launcher(launcher),
      file_importer(file_importer),
      file_loader(file_loader),
      color(color),
      project_level_router(project_level_router),
      ui_root_level_router(ui_root_level_router) {
}

std::shared_ptr<app_level> app_level::make_shared() {
    auto const worker = worker::make_shared();
    auto const system_url = system_url::make_shared();
    return std::make_shared<app_level>(
        worker, system_url, app_launcher::make_shared(worker, system_url),
        file_importer::make_shared(worker, static_cast<uint32_t>(worker_priority::file_importing)),
        file_loader::make_shared(), ae::color::make_shared(), project_level_router::make_shared(),
        ui_root_level_router::make_shared());
}

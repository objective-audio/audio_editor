//
//  ae_app_lifetime.cpp
//

#include "ae_app_lifetime.h"

#include <audio_editor_core/ae_app_dialog_lifecycle.h>
#include <audio_editor_core/ae_app_launcher.h>
#include <audio_editor_core/ae_app_lifetime_types.h>
#include <audio_editor_core/ae_color.h>
#include <audio_editor_core/ae_file_importer.h>
#include <audio_editor_core/ae_file_info_loader.h>
#include <audio_editor_core/ae_project_lifecycle.h>
#include <audio_editor_core/ae_project_preparer.h>
#include <audio_editor_core/ae_system_url.h>
#include <audio_editor_core/ae_ui_root_lifecycle.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<app_lifetime> app_lifetime::make_shared() {
    auto const worker = worker::make_shared();
    auto const system_url = system_url::make_shared();
    auto const file_info_loader = file_info_loader::make_shared();
    auto const project_lifecycle = project_lifecycle::make_shared();

    return std::make_shared<app_lifetime>(
        worker, system_url, app_launcher::make_shared(worker, system_url),
        file_importer::make_shared(worker, static_cast<uint32_t>(worker_priority::file_importing)), file_info_loader,
        ae::color::make_shared(), project_lifecycle, app_dialog_lifecycle::make_shared(),
        ui_root_lifecycle::make_shared(),
        project_preparer::make_shared(file_info_loader.get(), project_lifecycle.get()));
}

app_lifetime::app_lifetime(std::shared_ptr<yas::worker> const &worker,
                           std::shared_ptr<ae::system_url> const &system_url,
                           std::shared_ptr<ae::app_launcher> const &launcher,
                           std::shared_ptr<ae::file_importer> const &file_importer,
                           std::shared_ptr<ae::file_info_loader> const &file_info_loader,
                           std::shared_ptr<ae::color> const &color,
                           std::shared_ptr<ae::project_lifecycle> const &project_lifecycle,
                           std::shared_ptr<app_dialog_lifecycle> const &dialog_lifecycle,
                           std::shared_ptr<ae::ui_root_lifecycle> const &ui_root_lifecycle,
                           std::shared_ptr<ae::project_preparer> const &project_preparer)
    : worker(worker),
      system_url(system_url),
      launcher(launcher),
      file_importer(file_importer),
      file_info_loader(file_info_loader),
      color(color),
      project_lifecycle(project_lifecycle),
      dialog_lifecycle(dialog_lifecycle),
      ui_root_lifecycle(ui_root_lifecycle),
      project_preparer(project_preparer) {
}

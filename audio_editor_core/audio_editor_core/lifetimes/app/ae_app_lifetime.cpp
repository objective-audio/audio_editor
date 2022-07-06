//
//  ae_app_lifetime.cpp
//

#include "ae_app_lifetime.h"

#include <audio_editor_core/ae_action_sender.h>
#include <audio_editor_core/ae_app_dialog_lifecycle.h>
#include <audio_editor_core/ae_app_launcher.h>
#include <audio_editor_core/ae_app_lifetime_types.h>
#include <audio_editor_core/ae_color.h>
#include <audio_editor_core/ae_file_importer.h>
#include <audio_editor_core/ae_file_info_loader.h>
#include <audio_editor_core/ae_hierarchy.h>
#include <audio_editor_core/ae_system_url.h>
#include <audio_editor_core/ae_ui_root_lifecycle.h>
#include <audio_editor_core/ae_window_lifecycle.h>
#include <audio_editor_core/ae_window_opener.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<app_lifetime> app_lifetime::make_shared() {
    auto const worker = worker::make_shared();
    auto const system_url = system_url::make_shared();
    auto const file_info_loader = file_info_loader::make_shared();
    auto const window_lifecycle = window_lifecycle::make_shared();
    auto const action_sender = action_sender::make_shared(hierarchy::app_lifecycle().get());

    return std::make_shared<app_lifetime>(
        worker, system_url, app_launcher::make_shared(worker, system_url),
        file_importer::make_shared(worker, static_cast<uint32_t>(worker_priority::file_importing)), file_info_loader,
        ae::color::make_shared(), window_lifecycle, app_dialog_lifecycle::make_shared(),
        ui_root_lifecycle::make_shared(), window_opener::make_shared(file_info_loader.get(), window_lifecycle.get()),
        action_sender);
}

app_lifetime::app_lifetime(
    std::shared_ptr<yas::worker> const &worker, std::shared_ptr<ae::system_url> const &system_url,
    std::shared_ptr<ae::app_launcher> const &launcher, std::shared_ptr<ae::file_importer> const &file_importer,
    std::shared_ptr<ae::file_info_loader> const &file_info_loader, std::shared_ptr<ae::color> const &color,
    std::shared_ptr<ae::window_lifecycle> const &window_lifecycle,
    std::shared_ptr<app_dialog_lifecycle> const &dialog_lifecycle,
    std::shared_ptr<ae::ui_root_lifecycle> const &ui_root_lifecycle,
    std::shared_ptr<ae::window_opener> const &window_opener, std::shared_ptr<ae::action_sender> const &action_sender)
    : worker(worker),
      system_url(system_url),
      launcher(launcher),
      file_importer(file_importer),
      file_info_loader(file_info_loader),
      color(color),
      window_lifecycle(window_lifecycle),
      dialog_lifecycle(dialog_lifecycle),
      ui_root_lifecycle(ui_root_lifecycle),
      window_opener(window_opener),
      action_sender(action_sender) {
}

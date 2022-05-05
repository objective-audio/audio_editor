//
//  ae_app_level.cpp
//

#include "ae_app_level.h"

#include <audio_editor_core/ae_app_launcher.h>
#include <audio_editor_core/ae_app_level_types.h>
#include <audio_editor_core/ae_color.h>
#include <audio_editor_core/ae_file_importer.h>
#include <audio_editor_core/ae_file_loader.h>
#include <audio_editor_core/ae_project_level_collector.h>
#include <audio_editor_core/ae_system_url.h>
#include <audio_editor_core/ae_ui_root_level_collector.h>

using namespace yas;
using namespace yas::ae;

app_level::app_level()
    : worker(worker::make_shared()),
      system_url(system_url::make_shared()),
      launcher(app_launcher::make_shared(worker, system_url)),
      file_importer(file_importer::make_shared(worker, static_cast<uint32_t>(worker_priority::file_importing))),
      file_loader(file_loader::make_shared()),
      color(ae::color::make_shared()),
      project_level_collector(project_level_collector::make_shared()),
      ui_root_level_collector(ui_root_level_collector::make_shared()) {
    this->launcher->launch();
}

std::shared_ptr<app_level> app_level::make_shared() {
    return std::shared_ptr<app_level>(new app_level{});
}

std::shared_ptr<app_level> const &app_level::global() {
    static std::shared_ptr<app_level> const _global = app_level::make_shared();
    return _global;
}

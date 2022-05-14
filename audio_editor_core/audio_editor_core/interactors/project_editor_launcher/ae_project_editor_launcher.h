//
//  ae_project_editor_launcher.h
//

#pragma once

#include <audio_editor_core/ae_project_id.h>
#include <cpp_utils/yas_url.h>

namespace yas::ae {
class file_module_loader;

struct project_editor_launcher final {
    [[nodiscard]] static std::shared_ptr<project_editor_launcher> make_shared(project_id const &project_id,
                                                                              file_module_loader *);

    project_editor_launcher(url const &file_url, file_module_loader *);

    void launch();

   private:
    url const _file_url;
    file_module_loader *const _file_module_loader;
};
}  // namespace yas::ae

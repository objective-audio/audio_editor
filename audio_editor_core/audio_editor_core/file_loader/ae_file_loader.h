//
//  ae_file_loader.h
//

#pragma once

#include <audio_editor_core/ae_file_info.h>
#include <audio_editor_core/ae_project_dependency.h>
#include <audio_editor_core/ae_project_editor_dependency.h>

namespace yas::ae {
struct file_loader final : file_loader_for_project, file_loader_for_project_editor {
    std::optional<file_info> load_file_info(url const &) const override;

    static std::shared_ptr<file_loader> make_shared();

   private:
    file_loader();

    file_loader(file_loader const &) = delete;
    file_loader(file_loader &&) = delete;
    file_loader &operator=(file_loader const &) = delete;
    file_loader &operator=(file_loader &&) = delete;
};
}  // namespace yas::ae

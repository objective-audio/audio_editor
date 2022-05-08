//
//  ae_file_loader.h
//

#pragma once

#include <audio_editor_core/ae_file_info.h>
#include <audio_editor_core/ae_project_editor_dependency.h>
#include <audio_editor_core/ae_project_launcher_dependency.h>

namespace yas::ae {
struct file_loader final : file_loader_for_project_launcher {
    [[nodiscard]] static std::shared_ptr<file_loader> make_shared();

    file_loader();

    [[nodiscard]] std::optional<file_info> load_file_info(url const &) const override;

   private:
    file_loader(file_loader const &) = delete;
    file_loader(file_loader &&) = delete;
    file_loader &operator=(file_loader const &) = delete;
    file_loader &operator=(file_loader &&) = delete;
};
}  // namespace yas::ae

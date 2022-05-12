//
//  ae_file_info_loader.h
//

#pragma once

#include <audio_editor_core/ae_file_info.h>
#include <audio_editor_core/ae_project_launcher_dependency.h>

namespace yas::ae {
struct file_info_loader final : file_info_loader_for_project_launcher {
    [[nodiscard]] static std::shared_ptr<file_info_loader> make_shared();

    file_info_loader();

    [[nodiscard]] std::optional<file_info> load_file_info(url const &) const override;

   private:
    file_info_loader(file_info_loader const &) = delete;
    file_info_loader(file_info_loader &&) = delete;
    file_info_loader &operator=(file_info_loader const &) = delete;
    file_info_loader &operator=(file_info_loader &&) = delete;
};
}  // namespace yas::ae

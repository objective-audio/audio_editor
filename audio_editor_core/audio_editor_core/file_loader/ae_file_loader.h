//
//  ae_file_loader.h
//

#pragma once

#include <audio_editor_core/ae_file_loader_types.h>
#include <audio_editor_core/ae_project_dependency.h>
#include <audio_editor_core/ae_timeline_editor_dependency.h>
#include <cpp_utils/yas_url.h>

#include <memory>
#include <optional>

namespace yas::ae {
struct file_loader final : project_file_loader_interface, timeline_file_loader_interface {
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

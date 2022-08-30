//
//  ae_file_info_loader.h
//

#pragma once

#include <audio_editor_core/ae_file_info.h>

#include <filesystem>
#include <memory>
#include <optional>

namespace yas::ae {
struct file_info_loader final {
    [[nodiscard]] static std::shared_ptr<file_info_loader> make_shared();

    file_info_loader();

    [[nodiscard]] std::optional<file_info> load_file_info(std::filesystem::path const &) const;

   private:
    file_info_loader(file_info_loader const &) = delete;
    file_info_loader(file_info_loader &&) = delete;
    file_info_loader &operator=(file_info_loader const &) = delete;
    file_info_loader &operator=(file_info_loader &&) = delete;
};
}  // namespace yas::ae

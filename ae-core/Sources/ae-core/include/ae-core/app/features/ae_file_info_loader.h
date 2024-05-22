//
//  ae_file_info_loader.h
//

#pragma once

#include <ae-core/app/value_types/ae_file_info.h>

#include <filesystem>
#include <memory>
#include <optional>

namespace yas::ae {
struct file_info_loader final {
    file_info_loader();

    [[nodiscard]] std::optional<file_info> load_file_info(std::filesystem::path const &) const;

   private:
    file_info_loader(file_info_loader const &) = delete;
    file_info_loader(file_info_loader &&) = delete;
    file_info_loader &operator=(file_info_loader const &) = delete;
    file_info_loader &operator=(file_info_loader &&) = delete;
};
}  // namespace yas::ae

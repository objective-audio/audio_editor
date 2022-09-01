//
//  ae_system_path.h
//

#pragma once

#include <filesystem>

namespace yas::ae {
struct system_path final {
    [[nodiscard]] std::filesystem::path document_directory() const;
    [[nodiscard]] std::filesystem::path app_directory() const;
    [[nodiscard]] std::filesystem::path playing_directory() const;

    [[nodiscard]] static std::shared_ptr<system_path> make_shared();

   private:
    system_path();

    system_path(system_path const &) = delete;
    system_path(system_path &&) = delete;
    system_path &operator=(system_path const &) = delete;
    system_path &operator=(system_path &&) = delete;

    void _log_app_directory() const;
};
}  // namespace yas::ae

//
//  ae_project_path.h
//

#pragma once

#include <filesystem>

namespace yas::ae {
struct project_path final {
    static std::shared_ptr<project_path> make_shared(std::filesystem::path const &root);

    project_path(std::filesystem::path const &root);

    [[nodiscard]] std::filesystem::path const &root_directory() const;
    [[nodiscard]] std::filesystem::path editing_files_directory() const;
    [[nodiscard]] std::filesystem::path editing_file() const;
    [[nodiscard]] std::filesystem::path playing_directory() const;
    [[nodiscard]] std::filesystem::path db_file() const;

   private:
    std::filesystem::path const _root;

    project_path(project_path const &) = delete;
    project_path(project_path &&) = delete;
    project_path &operator=(project_path const &) = delete;
    project_path &operator=(project_path &&) = delete;
};
}  // namespace yas::ae

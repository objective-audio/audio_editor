//
//  ae_project_url.h
//

#pragma once

#include <filesystem>

namespace yas::ae {
struct project_url final {
    static std::shared_ptr<project_url> make_shared(std::filesystem::path const &root);

    project_url(std::filesystem::path const &root);

    std::filesystem::path const &root_directory() const;
    std::filesystem::path editing_files_directory() const;
    std::filesystem::path editing_file() const;
    std::filesystem::path playing_directory() const;
    std::filesystem::path db_file() const;

   private:
    std::filesystem::path const _root;

    project_url(project_url const &) = delete;
    project_url(project_url &&) = delete;
    project_url &operator=(project_url const &) = delete;
    project_url &operator=(project_url &&) = delete;
};
}  // namespace yas::ae

//
//  ae_project_setup_presenter.h
//

#pragma once

#include <filesystem>
#include <memory>

namespace yas::ae {
class project_setup;

struct project_setup_presenter final {
    [[nodiscard]] static std::shared_ptr<project_setup_presenter> make_shared();

    project_setup_presenter(std::shared_ptr<project_setup> const &);

    void select_directory(std::filesystem::path const &file_path);
    void did_close_dialog();

   private:
    std::weak_ptr<project_setup> const _setup;
};
}  // namespace yas::ae

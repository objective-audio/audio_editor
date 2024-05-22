//
//  ae_file_module_loader.h
//

#pragma once

#include <ae-core/app/value_types/ae_project_format.h>
#include <ae-core/global/value_types/ae_project_guid.h>

#include <filesystem>
#include <memory>

namespace yas::ae {
class project_path;
class file_importer;
class file_info_loader;
class file_module_loading_state_holder;
class database;
class uuid_generatable;
class pasteboard;

struct file_module_loader final : std::enable_shared_from_this<file_module_loader> {
    [[nodiscard]] static std::shared_ptr<file_module_loader> make_shared(project_guid const &, project_path const *,
                                                                         project_format const &,
                                                                         file_module_loading_state_holder *, database *,
                                                                         pasteboard *);

    file_module_loader(std::shared_ptr<uuid_generatable> const &, project_guid const &, project_path const *,
                       project_format const &, file_importer *, file_info_loader const *,
                       file_module_loading_state_holder *, database *, pasteboard *);

    void load(std::filesystem::path const &src_path);

   private:
    std::shared_ptr<uuid_generatable> const _uuid_generator;
    project_guid const _project_id;
    project_path const *const _project_path;
    project_format const _project_format;
    file_importer *const _file_importer;
    file_info_loader const *const _file_info_loader;
    file_module_loading_state_holder *const _state_holder;
    database *const _database;
    pasteboard *const _pasteboard;

    file_module_loader(file_module_loader const &) = delete;
    file_module_loader(file_module_loader &&) = delete;
    file_module_loader &operator=(file_module_loader const &) = delete;
    file_module_loader &operator=(file_module_loader &&) = delete;
};
}  // namespace yas::ae

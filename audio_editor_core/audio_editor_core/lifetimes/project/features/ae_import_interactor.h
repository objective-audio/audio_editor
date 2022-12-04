//
//  ae_import_interactor.h
//

#pragma once

#include <filesystem>
#include <memory>

namespace yas::ae {
class project_modal_lifecycle;
class editing_status;
class file_module_loader;

struct import_interactor final {
    import_interactor(project_modal_lifecycle *, editing_status const *, file_module_loader *);

    [[nodiscard]] bool can_select_file_for_import() const;
    void select_file_for_import();
    [[nodiscard]] bool can_import_from_file() const;
    void import_from_file(std::filesystem::path const &);

   private:
    project_modal_lifecycle *const _lifecycle;
    editing_status const *const _editing_status;
    file_module_loader *const _file_module_loader;
};
}  // namespace yas::ae

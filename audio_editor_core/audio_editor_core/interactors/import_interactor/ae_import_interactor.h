//
//  ae_import_interactor.h
//

#pragma once

#include <cpp_utils/yas_url.h>

#include <memory>

namespace yas::ae {
class project_sub_level_router;
class editing_status;
class file_module_loader;

struct import_interactor final {
    [[nodiscard]] static std::shared_ptr<import_interactor> make_shared(project_sub_level_router *,
                                                                        editing_status const *, file_module_loader *);

    import_interactor(project_sub_level_router *, editing_status const *, file_module_loader *);

    [[nodiscard]] bool can_select_file_for_import() const;
    void select_file_for_import();
    [[nodiscard]] bool can_import_from_file() const;
    void import_from_file(url const &file_url);

   private:
    project_sub_level_router *const _router;
    editing_status const *const _editing_status;
    file_module_loader *const _file_module_loader;
};
}  // namespace yas::ae

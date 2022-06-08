//
//  ae_project_preparer.h
//

#pragma once

#include <cpp_utils/yas_url.h>

#include <memory>

namespace yas::ae {
class uuid_generatable;
class system_url;
class file_info_loader;
class project_lifecycle;
class project_format;

struct project_preparer final {
    [[nodiscard]] static std::shared_ptr<project_preparer> make_shared(file_info_loader const *, ae::system_url const *,
                                                                       project_lifecycle *);

    project_preparer(std::shared_ptr<uuid_generatable> const &, system_url const *, file_info_loader const *,
                     project_lifecycle *);

    void prepare(url const &file_url);
    void prepare(project_format const &, url const &project_url);

   private:
    std::shared_ptr<uuid_generatable> const _uuid_generator;
    system_url const *const _system_url;
    file_info_loader const *const _file_info_loader;
    project_lifecycle *const _project_lifecycle;
};
}  // namespace yas::ae

//
//  module_name_presenter.h
//

#pragma once

#include <ae-core/global/value_types/common_types.h>
#include <ae-core/global/value_types/project_sub_lifetime_id.h>

#include <memory>
#include <string>

namespace yas::ae {
class module_name_editor;
class project_modal_lifecycle;

struct module_name_presenter final {
    [[nodiscard]] static std::shared_ptr<module_name_presenter> make_shared(project_sub_lifetime_id const &);

    module_name_presenter(project_sub_lifetime_id const &, std::shared_ptr<module_name_editor> const &,
                          std::shared_ptr<project_modal_lifecycle> const &);

    std::string const &name() const;

    void done(std::string const &);
    void cancel();

   private:
    project_sub_lifetime_id const _lifetime_id;
    std::weak_ptr<module_name_editor> _module_name_editor;
    std::weak_ptr<project_modal_lifecycle> _lifecycle;

    module_name_presenter(module_name_presenter const &) = delete;
    module_name_presenter(module_name_presenter &&) = delete;
    module_name_presenter &operator=(module_name_presenter const &) = delete;
    module_name_presenter &operator=(module_name_presenter &&) = delete;

    void _finalize();
};
}  // namespace yas::ae

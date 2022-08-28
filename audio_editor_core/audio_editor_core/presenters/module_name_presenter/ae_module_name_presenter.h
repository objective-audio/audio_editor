//
//  ae_module_name_presenter.h
//

#pragma once

#include <audio_editor_core/ae_common_types.h>
#include <audio_editor_core/ae_project_sub_lifetime_id.h>

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

    void _finalize();
};
}  // namespace yas::ae

//
//  ae_module_renaming_launcher.h
//

#pragma once

#include <audio_editor_core/ae_project_id.h>

#include <memory>

namespace yas::ae {
class project_sub_level_router;
class editing_status;

struct module_renaming_launcher final {
    [[nodiscard]] static std::shared_ptr<module_renaming_launcher> make_shared(project_sub_level_router *,
                                                                               editing_status const *);

    module_renaming_launcher(project_sub_level_router *, editing_status const *);

    [[nodiscard]] bool can_begin_module_renaming() const;
    void begin_module_renaming(std::string const &range);

   private:
    project_sub_level_router *const _router;
    editing_status const *const _editing_status;
};
}  // namespace yas::ae

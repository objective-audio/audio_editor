//
//  ae_module_renaming_launcher.h
//

#pragma once

#include <memory>

namespace yas::ae {
class project_modal_lifecycle;
class editing_status;

struct module_renaming_launcher final {
    [[nodiscard]] static std::shared_ptr<module_renaming_launcher> make_shared(project_modal_lifecycle *,
                                                                               editing_status const *);

    module_renaming_launcher(project_modal_lifecycle *, editing_status const *);

    [[nodiscard]] bool can_begin_module_renaming() const;
    void begin_module_renaming(std::string const &range);

   private:
    project_modal_lifecycle *const _lifecycle;
    editing_status const *const _editing_status;
};
}  // namespace yas::ae

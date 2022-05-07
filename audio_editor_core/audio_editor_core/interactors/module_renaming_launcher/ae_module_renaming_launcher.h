//
//  ae_module_renaming_launcher.h
//

#pragma once

#include <audio_editor_core/ae_project_id.h>

#include <memory>

namespace yas::ae {
class sheet_presenter;
class editing_status;

struct module_renaming_launcher final {
    [[nodiscard]] static std::shared_ptr<module_renaming_launcher> make_shared(project_id const &,
                                                                               editing_status const *);

    [[nodiscard]] bool can_begin_module_renaming() const;
    void begin_module_renaming(std::string const &range);

   private:
    sheet_presenter *const _sheet_presenter;
    editing_status const *const _editing_status;

    module_renaming_launcher(sheet_presenter *, editing_status const *);
};
}  // namespace yas::ae

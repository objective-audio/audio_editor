//
//  ae_module_renaming_opener.h
//

#pragma once

#include <audio_editor_core/ae_common_types.h>

#include <audio_editor_core/ae_file_module_index.hpp>

namespace yas::ae {
class project_modal_lifecycle;
class editing_status;
class deselector;

struct module_renaming_opener final {
    module_renaming_opener(project_modal_lifecycle *, editing_status const *, deselector *);

    [[nodiscard]] bool can_begin_module_renaming() const;
    void begin_module_renaming(file_module_index const &);

   private:
    project_modal_lifecycle *const _lifecycle;
    editing_status const *const _editing_status;
    deselector *const _deselector;

    module_renaming_opener(module_renaming_opener const &) = delete;
    module_renaming_opener(module_renaming_opener &&) = delete;
    module_renaming_opener &operator=(module_renaming_opener const &) = delete;
    module_renaming_opener &operator=(module_renaming_opener &&) = delete;
};
}  // namespace yas::ae

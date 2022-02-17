//
//  ae_ui_editing_root_maker.h
//

#pragma once

#include <audio_editor_core/ae_ui_root_dependency.h>
#include <ui/yas_ui_umbrella.h>

namespace yas::ae {
class ui_editing_root;

struct ui_editing_root_maker final : ui_editing_root_maker_for_ui_root {
    [[nodiscard]] static std::shared_ptr<ui_editing_root_maker> make_shared(std::string const &project_id,
                                                                            std::uintptr_t const project_view_id);

    std::shared_ptr<ui_editing_root> make() const override;

   private:
    std::string const _project_id;
    std::uintptr_t const _project_view_id;

    ui_editing_root_maker(std::string const &project_id, std::uintptr_t const project_view_id);
};
}  // namespace yas::ae

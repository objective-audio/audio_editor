//
//  ae_ui_editing_root_maker.h
//

#pragma once

#include <audio_editor_core/ae_ui_root_dependency.h>
#include <ui/yas_ui_umbrella.h>

namespace yas::ae {
class ui_editing_root;

struct ui_editing_root_maker final : ui_editing_root_maker_for_ui_root {
    [[nodiscard]] static std::shared_ptr<ui_editing_root_maker> make_shared(std::shared_ptr<ui::standard> const &,
                                                                            std::string const &project_id);

    std::shared_ptr<ui_editing_root> make() const override;

   private:
    std::shared_ptr<ui::standard> const _standard;
    std::string const _project_id;

    ui_editing_root_maker(std::shared_ptr<ui::standard> const &, std::string const &project_id);
};
}  // namespace yas::ae

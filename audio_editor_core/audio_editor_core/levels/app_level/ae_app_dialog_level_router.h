//
//  ae_app_dialog_level_router.h
//

#pragma once

#include <audio_editor_core/ae_app_dialog_content.h>
#include <observing/yas_observing_umbrella.h>

namespace yas::ae {
class app_dialog_level;

struct app_dialog_level_router final {
    [[nodiscard]] static std::shared_ptr<app_dialog_level_router> make_shared();

    app_dialog_level_router();

    void add_dialog(app_dialog_content const);
    void remove_dialog(app_dialog_content const);
    std::shared_ptr<app_dialog_level> const &dialog_level() const;

    observing::syncable observe(std::function<void(std::optional<std::shared_ptr<app_dialog_level>> const &)> &&);

   private:
    observing::value::holder_ptr<std::optional<std::shared_ptr<app_dialog_level>>> const _level;
};
}  // namespace yas::ae

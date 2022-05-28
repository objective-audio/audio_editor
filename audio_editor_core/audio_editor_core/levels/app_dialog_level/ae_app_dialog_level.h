//
//  ae_app_dialog_level.h
//

#pragma once

#include <audio_editor_core/ae_app_dialog_content.h>

#include <memory>

namespace yas::ae {
struct app_dialog_level {
    [[nodiscard]] static std::shared_ptr<app_dialog_level> make_shared();

    app_dialog_level();
};
}  // namespace yas::ae

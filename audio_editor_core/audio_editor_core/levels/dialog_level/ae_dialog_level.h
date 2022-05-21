//
//  ae_dialog_level.h
//

#pragma once

#include <audio_editor_core/ae_dialog_content.h>

#include <memory>

namespace yas::ae {
struct dialog_level final {
    [[nodiscard]] static std::shared_ptr<dialog_level> make_shared(dialog_content const);

    dialog_level(dialog_content const);

    dialog_content const content;
};
}  // namespace yas::ae

//
//  ae_dialog_lifetime.h
//

#pragma once

#include <audio_editor_core/ae_dialog_content.h>

#include <memory>

namespace yas::ae {
struct dialog_lifetime final {
    [[nodiscard]] static std::shared_ptr<dialog_lifetime> make_shared(dialog_content const);

    dialog_lifetime(dialog_content const);

    dialog_content const content;
};
}  // namespace yas::ae

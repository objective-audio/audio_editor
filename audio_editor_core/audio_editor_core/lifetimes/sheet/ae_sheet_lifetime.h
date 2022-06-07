//
//  ae_sheet_lifetime.h
//

#pragma once

#include <audio_editor_core/ae_sheet_content.h>

#include <memory>

namespace yas::ae {
struct sheet_lifetime final {
    [[nodiscard]] static std::shared_ptr<sheet_lifetime> make_shared(sheet_content const &);

    sheet_lifetime(sheet_content const &);

    sheet_content const content;
};
}  // namespace yas::ae

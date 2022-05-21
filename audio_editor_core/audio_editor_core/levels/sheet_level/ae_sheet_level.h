//
//  ae_sheet_level.h
//

#pragma once

#include <audio_editor_core/ae_sheet_content.h>

#include <memory>

namespace yas::ae {
struct sheet_level final {
    [[nodiscard]] static std::shared_ptr<sheet_level> make_shared(sheet_content const &);

    sheet_level(sheet_content const &);

    sheet_content const content;
};
}  // namespace yas::ae

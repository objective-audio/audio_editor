//
//  ae_pasteboard.h
//

#pragma once

#include <audio_editor_core/ae_pasteboard_types.h>
#include <audio_editor_core/ae_project_editor_dependency.h>

namespace yas::ae {
struct pasteboard final : pasteboard_for_project_editor {
    [[nodiscard]] static std::shared_ptr<pasteboard> make_shared();

    std::optional<pasting_file_module> file_module() const override;
    void set_file_module(pasting_file_module const &) override;

   private:
    std::string _data = "";

    pasteboard();
};
}  // namespace yas::ae

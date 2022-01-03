//
//  ae_nudging.h
//

#pragma once

#include <audio_editor_core/ae_nudging_types.h>
#include <audio_editor_core/ae_project_editor_dependency.h>

namespace yas::ae {
struct nudging final : nudging_for_project_editor {
    [[nodiscard]] static std::shared_ptr<nudging> make_shared();

    void set_kind(nudging_kind const);
    nudging_kind kind() const;

   private:
    nudging_kind _kind;

    nudging();
};
}  // namespace yas::ae

//
//  ae_nudging.h
//

#pragma once

#include <audio_editor_core/ae_nudging_types.h>
#include <audio_editor_core/ae_project_editor_dependency.h>
#include <processing/yas_processing_common_types.h>

namespace yas::ae {
struct nudging final : nudging_for_project_editor {
    [[nodiscard]] static std::shared_ptr<nudging> make_shared(proc::sample_rate_t const);

    void set_kind(nudging_kind const);
    nudging_kind kind() const;

   private:
    proc::sample_rate_t const _sample_rate;
    nudging_kind _kind;

    nudging(proc::sample_rate_t const);
};
}  // namespace yas::ae

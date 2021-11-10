//
//  ae_uuid_generator.h
//

#pragma once

#include <audio_editor_core/ae_project_dependency.h>

namespace yas::ae {
struct uuid_generator final : uuid_generator_for_project_maker {
    std::string generate() const override;

    static std::shared_ptr<uuid_generator> make_shared();

   private:
    uuid_generator() = default;
};
}  // namespace yas::ae

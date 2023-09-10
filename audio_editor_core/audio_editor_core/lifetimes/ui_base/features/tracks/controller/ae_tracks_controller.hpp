//
//  ae_tracks_controller.hpp
//

#pragma once

#include <audio_editor_core/ae_project_lifetime_id.h>

#include <memory>

namespace yas::ae {
struct tracks_controller final {
    [[nodiscard]] static std::shared_ptr<tracks_controller> make_shared(project_lifetime_id const &);

    tracks_controller(project_lifetime_id const &);
};
}  // namespace yas::ae

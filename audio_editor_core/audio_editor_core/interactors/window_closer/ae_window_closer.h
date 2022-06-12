//
//  ae_window_closer.h
//

#pragma once

#include <audio_editor_core/ae_project_id.h>

#include <memory>

namespace yas::ae {
class window_lifecycle;

struct window_closer final {
    [[nodiscard]] static std::shared_ptr<window_closer> make_shared(project_id const &, window_lifecycle *);

    window_closer(project_id const &, window_lifecycle *);

    [[nodiscard]] bool can_close() const;
    void close_if_needed();

   private:
    project_id const _project_id;
    window_lifecycle *_window_lifecycle;
};
}  // namespace yas::ae

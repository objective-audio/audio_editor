//
//  ae_marker_name_editor.hpp
//

#pragma once

#include <audio_editor_core/ae_project_sub_lifetime_id.h>

namespace yas::ae {
class marker_pool;

struct marker_name_editor final {
    [[nodiscard]] static std::shared_ptr<marker_name_editor> make_shared(project_sub_lifetime_id const &,
                                                                         int64_t const marker_frame);
    marker_name_editor(int64_t const marker_frame, marker_pool *);

    std::string const &name() const;
    void set_name(std::string const &);

   private:
    int64_t const _marker_frame;
    marker_pool *const _marker_pool;
};
}  // namespace yas::ae

//
//  ae_marker_name_presenter.hpp
//

#pragma once

#include <audio_editor_core/ae_window_sub_lifetime_id.h>

namespace yas::ae {
class marker_pool;
class project_modal_lifecycle;

struct marker_name_presenter final {
    [[nodiscard]] static std::shared_ptr<marker_name_presenter> make_shared(sheet_lifetime_id const &, int64_t const);
    marker_name_presenter(sheet_lifetime_id const &, int64_t const, std::shared_ptr<marker_pool> const &,
                          std::shared_ptr<project_modal_lifecycle> const &);

    [[nodiscard]] std::string name() const;

    void done(std::string const &);
    void cancel();

   private:
    sheet_lifetime_id const _lifetime_id;
    int64_t const _marker_frame;
    std::weak_ptr<marker_pool> _marker_pool;
    std::weak_ptr<project_modal_lifecycle> _lifecycle;

    void _finalize();
};
}  // namespace yas::ae

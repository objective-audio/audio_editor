//
//  ae_marker_name_presenter.hpp
//

#pragma once

#include <audio_editor_core/ae_project_sub_lifetime_id.h>

namespace yas::ae {
class marker_name_editor;
class project_modal_lifecycle;

struct marker_name_presenter final {
    [[nodiscard]] static std::shared_ptr<marker_name_presenter> make_shared(sheet_lifetime_id const &);
    marker_name_presenter(sheet_lifetime_id const &, std::shared_ptr<marker_name_editor> const &,
                          std::shared_ptr<project_modal_lifecycle> const &);

    [[nodiscard]] std::string const &name() const;

    void done(std::string const &);
    void cancel();

   private:
    sheet_lifetime_id const _lifetime_id;
    std::weak_ptr<marker_name_editor> _editor;
    std::weak_ptr<project_modal_lifecycle> _lifecycle;

    void _finalize();
};
}  // namespace yas::ae

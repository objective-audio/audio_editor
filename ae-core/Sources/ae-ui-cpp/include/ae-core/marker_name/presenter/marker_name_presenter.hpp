//
//  marker_name_presenter.hpp
//

#pragma once

#include <ae-core/global/value_types/project_sub_lifetime_id.h>

namespace yas::ae {
class marker_name_editor;
class project_modal_lifecycle;

struct marker_name_presenter final {
    [[nodiscard]] static std::shared_ptr<marker_name_presenter> make_shared(project_sub_lifetime_id const &);
    marker_name_presenter(project_sub_lifetime_id const &, std::shared_ptr<marker_name_editor> const &,
                          std::shared_ptr<project_modal_lifecycle> const &);

    [[nodiscard]] std::string name() const;

    void done(std::string const &);
    void cancel();

   private:
    project_sub_lifetime_id const _lifetime_id;
    std::weak_ptr<marker_name_editor> _editor;
    std::weak_ptr<project_modal_lifecycle> _lifecycle;

    marker_name_presenter(marker_name_presenter const &) = delete;
    marker_name_presenter(marker_name_presenter &&) = delete;
    marker_name_presenter &operator=(marker_name_presenter const &) = delete;
    marker_name_presenter &operator=(marker_name_presenter &&) = delete;

    void _finalize();
};
}  // namespace yas::ae

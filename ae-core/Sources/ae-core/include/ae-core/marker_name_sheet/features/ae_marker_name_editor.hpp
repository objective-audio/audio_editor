//
//  ae_marker_name_editor.hpp
//

#pragma once

#include <ae-core/global/value_types/ae_project_sub_lifetime_id.h>

#include <ae-core/project_editing/value_types/ae_marker_index.hpp>

namespace yas::ae {
class marker_pool;

struct marker_name_editor final {
    [[nodiscard]] static std::shared_ptr<marker_name_editor> make_shared(project_sub_lifetime_id const &,
                                                                         marker_index const &);
    marker_name_editor(marker_index const &, marker_pool *);

    [[nodiscard]] std::string name() const;
    void set_name(std::string const &);

   private:
    marker_index const _marker_index;
    marker_pool *const _marker_pool;

    marker_name_editor(marker_name_editor const &) = delete;
    marker_name_editor(marker_name_editor &&) = delete;
    marker_name_editor &operator=(marker_name_editor const &) = delete;
    marker_name_editor &operator=(marker_name_editor &&) = delete;
};
}  // namespace yas::ae

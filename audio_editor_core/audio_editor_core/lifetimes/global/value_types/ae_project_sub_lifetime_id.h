//
//  ae_project_sub_lifetime_id.h
//

#pragma once

#include <audio_editor_core/ae_project_lifetime_id.h>

namespace yas::ae {
struct project_sub_lifetime_id final {
    identifier instance;
    project_lifetime_id project;

    bool operator==(project_sub_lifetime_id const &) const;
    bool operator!=(project_sub_lifetime_id const &) const;
    bool operator<(project_sub_lifetime_id const &) const;
};
}  // namespace yas::ae

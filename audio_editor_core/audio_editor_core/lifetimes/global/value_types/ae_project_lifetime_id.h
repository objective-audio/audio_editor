//
//  ae_project_lifetime_id.h
//

#pragma once

#include <audio_editor_core/ae_project_guid.h>
#include <cpp_utils/yas_identifier.h>

namespace yas::ae {
struct project_lifetime_id final {
    identifier instance;
    project_guid guid;

    bool operator==(project_lifetime_id const &) const;
    bool operator!=(project_lifetime_id const &) const;
    bool operator<(project_lifetime_id const &) const;
};
}  // namespace yas::ae

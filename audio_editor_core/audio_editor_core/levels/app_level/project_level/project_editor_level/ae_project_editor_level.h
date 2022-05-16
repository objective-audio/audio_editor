//
//  ae_project_editor_level.h
//

#pragma once

#include <audio_editor_core/ae_common_types.h>
#include <audio_editor_core/ae_file_info.h>
#include <audio_editor_core/ae_project_format.h>
#include <audio_editor_core/ae_project_id.h>

#include <memory>
#include <string>

namespace yas::ae {
class file_info;
class project_url;
class project_level;

struct project_editor_level final {
    [[nodiscard]] static std::shared_ptr<project_editor_level> make_shared(ae::project_id const &project_id);

    project_editor_level(ae::project_id const &, project_format const &, std::shared_ptr<project_url> const &,
                         std::shared_ptr<project_level> const &);

    ae::project_id const project_id;
};
}  // namespace yas::ae

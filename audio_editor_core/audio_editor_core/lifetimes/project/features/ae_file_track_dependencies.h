//
//  ae_file_track_dependencies.h
//

#pragma once

#include <audio_editor_core/ae_file_module.h>

namespace yas::ae {
class db_module;

struct database_for_file_track {
    virtual ~database_for_file_track() = default;

    [[nodiscard]] virtual db_module add_module(file_module const &) = 0;
    virtual void remove_module(object_id const &) = 0;
    virtual void update_module(object_id const &, file_module const &) = 0;
};
}  // namespace yas::ae

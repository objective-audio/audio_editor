//
//  ae_file_track_dependencies.h
//

#pragma once

#include <audio_editor_core/ae_file_module.h>

namespace yas::ae {
class db_module;

struct database_for_file_track {
    virtual ~database_for_file_track() = default;

    [[nodiscard]] virtual db_module add_module(file_module_object::params const &) = 0;
    virtual void remove_module(time::range const &) = 0;
    virtual void update_module(time::range const &, file_module_object const &) = 0;
};
}  // namespace yas::ae

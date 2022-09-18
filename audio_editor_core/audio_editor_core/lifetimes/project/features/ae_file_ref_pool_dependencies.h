//
//  ae_file_ref_pool_dependencies.h
//

#pragma once

#include <audio_editor_core/ae_file_ref.hpp>

namespace yas::ae {
class db_file_ref;

struct database_for_file_ref_pool {
    virtual ~database_for_file_ref_pool() = default;

    [[nodiscard]] virtual std::optional<db_file_ref> add_file_ref(file_ref::params const &) = 0;
};
}  // namespace yas::ae

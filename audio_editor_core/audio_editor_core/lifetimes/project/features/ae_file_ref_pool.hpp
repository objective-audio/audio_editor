//
//  ae_file_ref_pool.hpp
//

#pragma once

#include <audio_editor_core/ae_file_ref_pool_dependencies.h>
#include <audio_editor_core/ae_file_ref_pool_types.h>
#include <observing/yas_observing_umbrella.h>

namespace yas::ae {
struct file_ref_pool final {
    file_ref_pool(database_for_file_ref_pool *);

    void revert(std::vector<file_ref_object> &&);
    void insert(file_ref_object::params const &);

    [[nodiscard]] std::optional<file_ref_object> ref(std::string const &) const;

   private:
    database_for_file_ref_pool *_database;

    observing::map::holder_ptr<std::string, file_ref_object> const _refs;
    observing::fetcher_ptr<file_ref_pool_event> _fetcher;
    observing::canceller_pool _pool;
};
}  // namespace yas::ae

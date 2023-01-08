//
//  ae_module_name_editor.hpp
//

#pragma once

#include <audio_editor_core/ae_common_types.h>
#include <audio_editor_core/ae_project_sub_lifetime_id.h>

#include <audio_editor_core/ae_module_index.hpp>

namespace yas::ae {
class module_pool;
class database;

struct module_name_editor final {
    [[nodiscard]] static std::shared_ptr<module_name_editor> make_shared(project_sub_lifetime_id const &,
                                                                         module_index const &);
    module_name_editor(module_index const &, module_pool *, database *);

    [[nodiscard]] std::string const &name() const;
    void set_name(std::string const &);

   private:
    module_index const _module_index;
    module_pool *const _module_pool;
    database *const _database;

    module_name_editor(module_name_editor const &) = delete;
    module_name_editor(module_name_editor &&) = delete;
    module_name_editor &operator=(module_name_editor const &) = delete;
    module_name_editor &operator=(module_name_editor &&) = delete;
};
}  // namespace yas::ae

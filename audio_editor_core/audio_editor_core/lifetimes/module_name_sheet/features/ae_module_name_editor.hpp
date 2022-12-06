//
//  ae_module_name_editor.hpp
//

#pragma once

#include <audio_editor_core/ae_common_types.h>
#include <audio_editor_core/ae_project_sub_lifetime_id.h>

#include <audio_editor_core/ae_file_module_index.hpp>

namespace yas::ae {
class file_track;
class database;

struct module_name_editor final {
    [[nodiscard]] static std::shared_ptr<module_name_editor> make_shared(project_sub_lifetime_id const &,
                                                                         file_module_index const &);
    module_name_editor(file_module_index const &, file_track *, database *);

    [[nodiscard]] std::string const &name() const;
    void set_name(std::string const &);

   private:
    file_module_index const _module_index;
    file_track *const _file_track;
    database *const _database;

    module_name_editor(module_name_editor const &) = delete;
    module_name_editor(module_name_editor &&) = delete;
    module_name_editor &operator=(module_name_editor const &) = delete;
    module_name_editor &operator=(module_name_editor &&) = delete;
};
}  // namespace yas::ae

//
//  ae_module_name_editor.hpp
//

#pragma once

#include <audio_editor_core/ae_common_types.h>
#include <audio_editor_core/ae_window_sub_lifetime_id.h>

namespace yas::ae {
class file_track;
class database;

struct module_name_editor final {
    [[nodiscard]] static std::shared_ptr<module_name_editor> make_shared(sheet_lifetime_id const &,
                                                                         time::range const &);
    module_name_editor(time::range const &, file_track *, database *);

    std::string const &name() const;
    void set_name(std::string const &);

   private:
    time::range const _module_range;
    file_track *const _file_track;
    database *const _database;
};
}  // namespace yas::ae

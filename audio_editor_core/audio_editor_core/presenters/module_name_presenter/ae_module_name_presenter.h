//
//  ae_module_name_presenter.h
//

#pragma once

#include <audio_editor_core/ae_common_types.h>
#include <audio_editor_core/ae_project_id.h>

#include <memory>
#include <string>

namespace yas::ae {
class file_track;

struct module_name_presenter final {
    [[nodiscard]] static std::shared_ptr<module_name_presenter> make_shared(project_id const &project_id,
                                                                            time::range const module_range);

    std::string const &name() const;
    void set_name(std::string const &);

   private:
    time::range const _module_range;
    std::weak_ptr<file_track> const _file_track;

    module_name_presenter(time::range const &, std::shared_ptr<file_track> const &);
};
}  // namespace yas::ae

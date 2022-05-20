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
class project_sub_level_router;

struct module_name_presenter final {
    [[nodiscard]] static std::shared_ptr<module_name_presenter> make_shared(project_id const &project_id,
                                                                            time::range const module_range);

    module_name_presenter(time::range const &, std::shared_ptr<file_track> const &,
                          std::shared_ptr<project_sub_level_router> const &);

    std::string const &name() const;
    void set_name(std::string const &);

    void done(std::string const &);
    void cancel();

   private:
    time::range const _module_range;
    std::weak_ptr<file_track> _file_track;
    std::weak_ptr<project_sub_level_router> _router;

    void _finalize();
};
}  // namespace yas::ae

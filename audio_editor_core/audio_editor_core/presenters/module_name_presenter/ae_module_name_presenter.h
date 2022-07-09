//
//  ae_module_name_presenter.h
//

#pragma once

#include <audio_editor_core/ae_common_types.h>
#include <audio_editor_core/ae_window_sub_lifetime_id.h>

#include <memory>
#include <string>

namespace yas::ae {
class file_track;
class project_modal_lifecycle;

struct module_name_presenter final {
    [[nodiscard]] static std::shared_ptr<module_name_presenter> make_shared(sheet_lifetime_id const &,
                                                                            time::range const module_range);

    module_name_presenter(sheet_lifetime_id const &, time::range const &, std::shared_ptr<file_track> const &,
                          std::shared_ptr<project_modal_lifecycle> const &);

    std::string const &name() const;

    void done(std::string const &);
    void cancel();

   private:
    sheet_lifetime_id const _lifetime_id;
    time::range const _module_range;
    std::weak_ptr<file_track> _file_track;
    std::weak_ptr<project_modal_lifecycle> _lifecycle;

    void _finalize();
};
}  // namespace yas::ae

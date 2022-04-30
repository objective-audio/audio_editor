//
//  ae_project_closer.h
//

#pragma once

#include <audio_editor_core/ae_project_types.h>
#include <observing/yas_observing_umbrella.h>

namespace yas::ae {
class file_importer;
class project_editor_level_pool;
class project_status;

struct project_closer final {
    [[nodiscard]] static std::shared_ptr<project_closer> make_shared(std::string const &project_id,
                                                                     std::shared_ptr<file_importer> const &,
                                                                     std::shared_ptr<project_editor_level_pool> const &,
                                                                     std::shared_ptr<project_status> const &);

    [[nodiscard]] bool can_close() const;
    void request_close();

    [[nodiscard]] observing::endable observe_event(std::function<void(project_event const &)> &&);

   private:
    std::string const _project_id;

    std::shared_ptr<file_importer> const _file_importer;
    std::shared_ptr<project_editor_level_pool> const _editor_level_pool;
    std::shared_ptr<project_status> const _status;

    observing::notifier_ptr<project_event> const _event_notifier;

    project_closer(std::string const &project_id, std::shared_ptr<file_importer> const &,
                   std::shared_ptr<project_editor_level_pool> const &, std::shared_ptr<project_status> const &);
};
}  // namespace yas::ae

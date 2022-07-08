//
//  ae_window_lifetime.cpp
//

#include "ae_window_lifetime.h"

#include <audio_editor_core/ae_hierarchy.h>
#include <audio_editor_core/ae_project_lifecycle.h>
#include <audio_editor_core/ae_project_url.h>
#include <audio_editor_core/ae_window_closer.h>
#include <audio_editor_core/ae_window_receiver.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<window_lifetime> window_lifetime::make_shared(window_lifetime_id const &lifetime_id,
                                                              ae::project_format const &project_format,
                                                              url const &project_dir_url) {
    auto const &app_lifetime = hierarchy::app_lifetime();

    return std::make_shared<window_lifetime>(
        lifetime_id, project_format, project_dir_url, ae::project_url::make_shared(project_dir_url),
        ae::project_lifecycle::make_shared(lifetime_id),
        ae::window_closer::make_shared(lifetime_id, app_lifetime->window_lifecycle.get()),
        ae::window_receiver::make_shared(lifetime_id));
}

window_lifetime::window_lifetime(window_lifetime_id const &lifetime_id, ae::project_format const &project_format,
                                 url const &project_dir_url, std::shared_ptr<ae::project_url> const &project_url,
                                 std::shared_ptr<ae::project_lifecycle> const &project_lifecycle,
                                 std::shared_ptr<ae::window_closer> const &window_closer,
                                 std::shared_ptr<window_receiver> const &receiver)
    : lifetime_id(lifetime_id),
      project_format(project_format),
      project_directory_url(project_dir_url),
      project_url(project_url),
      project_lifecycle(project_lifecycle),
      closer(window_closer),
      receiver(receiver) {
}

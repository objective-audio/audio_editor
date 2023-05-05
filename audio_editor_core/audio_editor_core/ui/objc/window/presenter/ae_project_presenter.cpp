//
//  ae_project_presenter.cpp
//

#include "ae_project_presenter.h"

#include <audio_editor_core/ae_hierarchy.h>
#include <audio_editor_core/ae_project_closer.h>
#include <audio_editor_core/ae_project_launching_lifetime.h>
#include <audio_editor_core/ae_project_lifetime.h>
#include <audio_editor_core/ae_project_path.h>
#include <audio_editor_core/ae_project_sub_lifecycle.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<project_presenter> project_presenter::make_shared(ae::project_lifetime_id const &lifetime_id) {
    auto const &project_lifetime = hierarchy::project_lifetime_for_id(lifetime_id);
    return std::make_shared<project_presenter>(lifetime_id, project_lifetime->project_path, project_lifetime->closer,
                                               project_lifetime->project_sub_lifecycle);
}

project_presenter::project_presenter(ae::project_lifetime_id const &lifetime_id,
                                     std::shared_ptr<project_path> const &project_path,
                                     std::shared_ptr<project_closer> const &closer,
                                     std::shared_ptr<project_sub_lifecycle> const &project_sub_lifecycle)
    : lifetime_id(lifetime_id),
      _project_path(project_path),
      _closer(closer),
      _project_sub_lifecycle(project_sub_lifecycle) {
}

std::string project_presenter::title() const {
    if (auto const project_path = this->_project_path.lock()) {
        return project_path->root_directory().filename();
    } else {
        return "";
    }
}

bool project_presenter::should_close() {
    auto const closer = this->_closer.lock();

    if (closer && closer->can_close()) {
        closer->close_if_needed();
        return true;
    } else {
        return false;
    }
}

observing::syncable project_presenter::observe(std::function<void(project_presenter_event const &)> &&handler) {
    if (auto const lifecycle = this->_project_sub_lifecycle.lock()) {
        return lifecycle->observe([once_called = false, handler = std::move(handler)](
                                      std::optional<project_sub_lifetime> const &current) mutable {
            if (once_called) {
                return;
            }

            using kind = project_sub_lifetime_kind;

            switch (to_kind(current)) {
                case kind::none:
                case kind::launching:
                    break;

                case kind::editing:
                    once_called = true;
                    handler(project_presenter_event::setup_view_controller);
                    break;
            }
        });
    } else {
        return observing::syncable{};
    }
}

//
//  ae_window_presenter.cpp
//

#include "ae_window_presenter.h"

#include <audio_editor_core/ae_hierarchy.h>
#include <audio_editor_core/ae_project_launch_lifetime.h>
#include <audio_editor_core/ae_project_lifecycle.h>
#include <audio_editor_core/ae_project_url.h>
#include <audio_editor_core/ae_window_closer.h>
#include <audio_editor_core/ae_window_lifetime.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<window_presenter> window_presenter::make_shared(ae::window_lifetime_id const &lifetime_id) {
    auto const &window_lifetime = hierarchy::window_lifetime_for_id(lifetime_id);
    return std::make_shared<window_presenter>(lifetime_id, window_lifetime->project_url, window_lifetime->closer,
                                              window_lifetime->project_lifecycle);
}

window_presenter::window_presenter(ae::window_lifetime_id const &lifetime_id,
                                   std::shared_ptr<project_url> const &project_url,
                                   std::shared_ptr<window_closer> const &closer,
                                   std::shared_ptr<project_lifecycle> const &project_lifecycle)
    : lifetime_id(lifetime_id), _project_url(project_url), _closer(closer), _project_lifecycle(project_lifecycle) {
}

std::string window_presenter::title() const {
    if (auto const project_url = this->_project_url.lock()) {
        return project_url->root_directory().filename();
    } else {
        return "";
    }
}

bool window_presenter::should_close() {
    auto const &closer = this->_closer.lock();

    if (closer && closer->can_close()) {
        closer->close_if_needed();
        return true;
    } else {
        return false;
    }
}

observing::syncable window_presenter::observe(std::function<void(window_presenter_event const &)> &&handler) {
    if (auto const lifecycle = this->_project_lifecycle.lock()) {
        return lifecycle->observe([once_called = false, handler = std::move(handler)](
                                      std::optional<project_sub_lifetime> const &current) mutable {
            if (once_called) {
                return;
            }

            using kind = project_sub_lifetime_kind;

            switch (to_kind(current)) {
                case kind::none:
                case kind::launch:
                    break;

                case kind::project:
                    once_called = true;
                    handler(window_presenter_event::setup_view_controller);
                    break;
            }
        });
    } else {
        return observing::syncable{};
    }
}

//
//  ae_project_lifecycle.cpp
//

#include "ae_project_lifecycle.h"

#include <audio_editor_core/ae_project_launcher.h>
#include <audio_editor_core/ae_project_lifetime.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<project_lifecycle> project_lifecycle::make_shared(ae::project_id const &project_id) {
    return std::make_shared<project_lifecycle>(project_id);
}

project_lifecycle::project_lifecycle(ae::project_id const &project_id)
    : project_id(project_id),
      _current(observing::value::holder<std::optional<project_sub_lifetime>>::make_shared(std::nullopt)) {
}

std::optional<project_sub_lifetime> const &project_lifecycle::current() const {
    return this->_current->value();
}

void project_lifecycle::switch_to_project() {
    auto const lifetime = project_lifetime::make_shared(this->project_id);

    this->_current->set_value(lifetime);

    lifetime->launcher->launch();
}

observing::syncable project_lifecycle::observe(
    std::function<void(std::optional<project_sub_lifetime> const &)> &&handler) {
    return this->_current->observe(std::move(handler));
}

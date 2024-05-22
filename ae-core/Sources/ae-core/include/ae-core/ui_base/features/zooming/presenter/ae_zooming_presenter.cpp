//
//  ae_zooming_presenter.cpp
//

#include "ae_zooming_presenter.h"

#include <ae-core/global/ae_hierarchy.h>
#include <ae-core/project/features/ae_player.h>
#include <ae-core/project/features/ae_zooming_pair.h>
#include <cpp-utils/stl_utils.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<zooming_presenter> zooming_presenter::make_shared(project_lifetime_id const &project_lifetime_id) {
    auto const &project_lifetime = hierarchy::project_lifetime_for_id(project_lifetime_id);
    return std::make_shared<zooming_presenter>(project_lifetime->zooming_pair);
}

zooming_presenter::zooming_presenter(std::shared_ptr<zooming_pair> const &zooming_pair) : _zooming_pair(zooming_pair) {
}

ae::zooming_scale zooming_presenter::zooming_scale() const {
    if (auto const zooming_pair = this->_zooming_pair.lock()) {
        return zooming_pair->scale();
    } else {
        return {1.0, 1.0};
    }
}

observing::syncable zooming_presenter::observe_zooming_scale(std::function<void(ae::zooming_scale const &)> &&handler) {
    if (auto const zooming_pair = this->_zooming_pair.lock()) {
        return zooming_pair->observe_scale(std::move(handler));
    } else {
        return observing::syncable{};
    }
}

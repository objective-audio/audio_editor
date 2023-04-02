//
//  ae_zooming_presenter.cpp
//

#include "ae_zooming_presenter.h"

#include <audio_editor_core/ae_hierarchy.h>
#include <audio_editor_core/ae_player.h>
#include <audio_editor_core/ae_zooming_pair.h>
#include <cpp_utils/yas_stl_utils.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<zooming_presenter> zooming_presenter::make_shared(window_lifetime_id const &window_lifetime_id) {
    auto const &window_lifetime = hierarchy::window_lifetime_for_id(window_lifetime_id);
    return std::make_shared<zooming_presenter>(window_lifetime->zooming_pair);
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

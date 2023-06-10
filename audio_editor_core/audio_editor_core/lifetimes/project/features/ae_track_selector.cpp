//
//  ae_track_selector.cpp
//

#include "ae_track_selector.hpp"

#include <audio_editor_core/ae_vertical_scrolling.hpp>

using namespace yas;
using namespace yas::ae;

track_selector::track_selector(scrolling_for_track_selector *scrolling)
    : scrolling(scrolling), _current(observing::value::holder<track_index_t>::make_shared(0)) {
    this->scrolling->observe_track([this](double const &track) { this->_current->set_value(std::round(track)); })
        .sync()
        ->add_to(this->_cancellers);
}

track_index_t track_selector::current() const {
    return this->_current->value();
}

observing::syncable track_selector::observe_current(std::function<void(track_index_t const &)> &&handler) {
    return this->_current->observe(std::move(handler));
}

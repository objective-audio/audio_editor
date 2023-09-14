//
//  ae_range_selector.cpp
//

#include "ae_range_selector.hpp"

using namespace yas;
using namespace yas::ae;

range_selector::range_selector(player_for_range_selector *player, deselector_for_range_selector *deselector)
    : _selection(observing::value::holder<range_selection>::make_shared({.phase = range_selection_phase::ended})),
      _player(player),
      _deselector(deselector) {
    this->_player
        ->observe_is_playing([this](bool const &is_playing) {
            if (is_playing) {
                this->end();
            }
        })
        .end()
        ->add_to(this->_pool);
}

void range_selector::begin(ui::point const &position) {
    if (!this->_player->is_playing()) {
        this->_selection->set_value(
            {.phase = range_selection_phase::began, .range = point_range{.first = position, .second = position}});
    }
}

void range_selector::move(ui::point const &position) {
    auto const &range = this->_selection->value().range;
    if (range.has_value() && !this->_player->is_playing()) {
        this->_selection->set_value(
            {.phase = range_selection_phase::moved, .range = range.value().second_updated(position)});
    }
}

void range_selector::end() {
    this->_selection->set_value({.phase = range_selection_phase::ended});
}

range_selection const &range_selector::selection() const {
    return this->_selection->value();
}

observing::syncable range_selector::observe(std::function<void(range_selection const &)> &&handler) const {
    return this->_selection->observe(std::move(handler));
}

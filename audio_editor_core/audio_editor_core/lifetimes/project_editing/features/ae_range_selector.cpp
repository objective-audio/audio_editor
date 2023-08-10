//
//  ae_range_selector.cpp
//

#include "ae_range_selector.hpp"

using namespace yas;
using namespace yas::ae;

range_selector::range_selector(player_for_range_selector *player, deselector_for_range_selector *deselector)
    : _range(observing::value::holder<std::optional<point_range>>::make_shared(std::nullopt)),
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
        this->_range->set_value(point_range{.first = position, .second = position});
    }
}

void range_selector::move(ui::point const &position) {
    if (this->_range->value().has_value() && !this->_player->is_playing()) {
        this->_range->set_value(this->_range->value().value().second_updated(position));
    }
}

void range_selector::end() {
    this->_range->set_value(std::nullopt);
}

std::optional<ui::region> range_selector::region() const {
    if (!this->_range->value().has_value()) {
        return std::nullopt;
    }

    return this->_range->value().value().region();
}

observing::syncable range_selector::observe_region(
    std::function<void(std::optional<ui::region> const &)> &&handler) const {
    return this->_range->observe([handler = std::move(handler)](std::optional<point_range> const &range) {
        if (range.has_value()) {
            handler(range.value().region());
        } else {
            handler(std::nullopt);
        }
    });
}

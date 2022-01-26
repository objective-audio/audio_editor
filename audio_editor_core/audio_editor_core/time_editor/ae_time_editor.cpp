//
//  ae_time_editor.cpp
//

#include "ae_time_editor.h"

#include <audio_editor_core/ae_math.h>
#include <audio_editor_core/ae_time_editor_utils.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<time_editor> time_editor::make_shared(number_components const &components) {
    return std::shared_ptr<time_editor>(new time_editor{components});
}

time_editor::time_editor(number_components const &components)
    : _state(state::editing),
      _original_components(components),
      _commited_components(time_editor_utils::to_editing_components(components)),
      _unit_idx(observing::value::holder<std::size_t>::make_shared(components.size() - 1)),
      _event_notifier(observing::notifier<time_editor_event>::make_shared()) {
    this->_components_fetcher =
        observing::fetcher<number_components>::make_shared([this] { return this->editing_components(); });
}

bool time_editor::can_input_number() const {
    if (this->_is_ended()) {
        return false;
    }

    auto const &unit = this->_commited_components.unit(this->_unit_idx->value());
    auto const digits = math::decimal_digits_from_size(unit.size);
    return this->_unit_numbers.size() < digits;
}

bool time_editor::can_delete_number() const {
    if (this->_is_ended()) {
        return false;
    }

    return this->_unit_numbers.size() > 0;
}

void time_editor::input_number(uint32_t const number) {
    if (!this->can_input_number()) {
        return;
    }

    assert(number < 10);

    this->_unit_numbers.emplace_back(std::to_string(number));
    this->_components_fetcher->push();
}

void time_editor::delete_number() {
#warning todo 何も無い時に呼んだら0を入れたい？
    if (!this->can_delete_number()) {
        return;
    }

    this->_unit_numbers.pop_back();
    this->_components_fetcher->push();
}

bool time_editor::can_move_to_next_unit() const {
    if (this->_is_ended()) {
        return false;
    }

    return this->_unit_idx->value() != 0;
}

bool time_editor::can_move_to_previous_unit() const {
    if (this->_is_ended()) {
        return false;
    }

    return this->_unit_idx->value() < (this->_commited_components.size() - 1);
}

void time_editor::move_to_next_unit() {
    if (!this->can_move_to_next_unit()) {
        return;
    }

    this->_commit_unit_value();

    this->_unit_idx->set_value(this->_unit_idx->value() - 1);
}

void time_editor::move_to_previous_unit() {
    if (!this->can_move_to_previous_unit()) {
        return;
    }

    this->_commit_unit_value();

    this->_unit_idx->set_value(this->_unit_idx->value() + 1);
}

void time_editor::change_sign_to_plus() {
    if (this->_is_ended()) {
        return;
    }

    if (this->_commited_components.is_minus()) {
        this->_commited_components = this->_commited_components.is_minus_replaced(false);
        this->_components_fetcher->push();
    }
}

void time_editor::change_sign_to_minus() {
    if (this->_is_ended()) {
        return;
    }

    if (!this->_commited_components.is_minus()) {
        this->_commited_components = this->_commited_components.is_minus_replaced(true);
        this->_components_fetcher->push();
    }
}

void time_editor::finish() {
    if (this->_is_ended()) {
        return;
    }

    this->_commit_unit_value();

    this->_state = state::finished;
    this->_event_notifier->notify(time_editor_event::finished);
}

void time_editor::cancel() {
    if (this->_is_ended()) {
        return;
    }

    this->_state = state::canceled;
    this->_event_notifier->notify(time_editor_event::canceled);
}

std::size_t time_editor::unit_index() const {
    return this->_unit_idx->value();
}

number_components time_editor::editing_components() const {
    if (auto const unit_value = this->_editing_unit_value()) {
        return this->_commited_components.unit_value_replaced(unit_value.value(), this->_unit_idx->value());
    } else {
        return this->_commited_components;
    }
}

std::optional<number_components> time_editor::finalized_components() const {
    if (this->_state == state::finished) {
        std::vector<number_components_unit> units;
        auto each = make_fast_each(this->_original_components.size());
        while (yas_each_next(each)) {
            auto const &idx = yas_each_index(each);
            auto const &original_unit = this->_original_components.unit(idx);
            auto const &commited_unit = this->_commited_components.unit(idx);
            units.emplace_back(number_components_unit{.size = original_unit.size, .value = commited_unit.value});
        }
        return number_components{this->_commited_components.is_minus(), std::move(units)};
    }

    return std::nullopt;
}

observing::syncable time_editor::observe_unit_index(std::function<void(std::size_t const &)> &&handler) {
    return this->_unit_idx->observe(std::move(handler));
}

observing::syncable time_editor::observe_editing_components(std::function<void(number_components const &)> &&handler) {
    return this->_components_fetcher->observe(std::move(handler));
}

observing::endable time_editor::observe_event(std::function<void(time_editor_event const &)> &&handler) {
    return this->_event_notifier->observe(std::move(handler));
}

std::optional<uint32_t> time_editor::_editing_unit_value() const {
    if (this->_unit_numbers.size() > 0) {
        auto const &unit_idx = this->_unit_idx->value();
        auto const &unit = this->_commited_components.unit(unit_idx);

        std::string joined_numbers;

        // 入力した最後の要素を桁数分だけ使う
        auto const count =
            std::min(static_cast<std::size_t>(math::decimal_digits_from_size(unit.size)), this->_unit_numbers.size());
        auto each = make_fast_each(count);
        while (yas_each_next(each)) {
            auto const idx = this->_unit_numbers.size() - yas_each_index(each) - 1;
            if (idx < this->_unit_numbers.size()) {
                joined_numbers.insert(0, this->_unit_numbers.at(idx));
            }
        }

        return std::stoi(joined_numbers);
    } else {
        return std::nullopt;
    }
}

std::optional<uint32_t> time_editor::_commited_unit_value() const {
    if (auto editing_value = this->_editing_unit_value()) {
        auto const &original_unit = this->_original_components.unit(this->_unit_idx->value());
        if (original_unit.size != 0 && original_unit.size <= editing_value.value()) {
            return original_unit.size - 1;
        } else {
            return editing_value;
        }
    } else {
        return std::nullopt;
    }
}

void time_editor::_commit_unit_value() {
#warning todo 超えた分は上のunitに繰り上げたい？
    if (auto const unit_value = this->_commited_unit_value()) {
        this->_commited_components =
            this->_commited_components.unit_value_replaced(unit_value.value(), this->_unit_idx->value());
        this->_unit_numbers.clear();
        this->_components_fetcher->push();
    }
}

bool time_editor::_is_ended() const {
    switch (this->_state) {
        case state::finished:
        case state::canceled:
            return true;

        case state::editing:
            return false;
    }
}
//
//  ae_time_editor.cpp
//

#include "ae_time_editor.h"

#include <audio_editor_core/ae_math.h>
#include <audio_editor_core/ae_time_editor_utils.h>
#include <cpp_utils/yas_fast_each.h>

using namespace yas;
using namespace yas::ae;

static std::vector<std::string> const _zero_string_vector{"0"};

std::shared_ptr<time_editor> time_editor::make_shared(number_components const &components) {
    return std::make_shared<time_editor>(components);
}

time_editor::time_editor(number_components const &components)
    : _original_components(components),
      _commited_components(time_editor_utils::to_editing_components(components)),
      _unit_idx(observing::value::holder<std::size_t>::make_shared(components.size() - 1)),
      _event_notifier(observing::notifier<time_editor_event>::make_shared()) {
    this->_components_fetcher =
        observing::fetcher<number_components>::make_shared([this] { return this->editing_components(); });
}

bool time_editor::can_input_number() const {
    auto const &unit = this->_commited_components.unit(this->_unit_idx->value());
    auto const digits = math::decimal_digits_from_size(unit.size);

    if (this->_unit_numbers == _zero_string_vector && digits > 0) {
        return true;
    } else {
        return this->_unit_numbers.size() < digits;
    }
}

bool time_editor::can_delete_number() const {
    return this->_unit_numbers != _zero_string_vector;
}

bool time_editor::can_increment_number() const {
    auto const components = this->editing_components();
    auto const &unit = components.unit(this->_unit_idx->value());
    return unit.value < (unit.size - 1);
}

bool time_editor::can_decrement_number() const {
    auto const components = this->editing_components();
    auto const &unit = components.unit(this->_unit_idx->value());
    return unit.value > 0;
}

void time_editor::input_number(uint32_t const number) {
    if (!this->can_input_number()) {
        return;
    }

    assert(number < 10);

    if (this->_unit_numbers == _zero_string_vector) {
        this->_unit_numbers.clear();
    }

    this->_unit_numbers.emplace_back(std::to_string(number));

    this->_components_fetcher->push();
}

void time_editor::delete_number() {
    if (!this->can_delete_number()) {
        return;
    }

    if (this->_unit_numbers.size() > 0) {
        this->_unit_numbers.pop_back();
    }

    if (this->_unit_numbers.size() == 0) {
        this->_unit_numbers.emplace_back("0");
    }

    this->_components_fetcher->push();
}

void time_editor::increment_number() {
    if (!this->can_increment_number()) {
        return;
    }

    auto const components = this->editing_components();
    auto const &unit = components.unit(this->_unit_idx->value());

    this->_update_unit_numbers(unit.value + 1);
}

void time_editor::decrement_number() {
    if (!this->can_decrement_number()) {
        return;
    }

    auto const components = this->editing_components();
    auto const &unit = components.unit(this->_unit_idx->value());

    this->_update_unit_numbers(unit.value - 1);
}

bool time_editor::can_move_to_next_unit() const {
    return this->_unit_idx->value() != 0;
}

bool time_editor::can_move_to_previous_unit() const {
    return this->_unit_idx->value() < (this->_commited_components.size() - 1);
}

bool time_editor::can_set_unit_idx() const {
    return true;
}

void time_editor::set_unit_idx(std::size_t const idx) {
    if (!this->can_set_unit_idx()) {
        return;
    }

    if (this->_unit_idx->value() != idx && idx < this->_original_components.size()) {
        this->commit_unit_value();

        this->_unit_idx->set_value(idx);
    }
}

void time_editor::move_to_next_unit() {
    if (!this->can_move_to_next_unit()) {
        return;
    }

    this->set_unit_idx(this->_unit_idx->value() - 1);
}

void time_editor::move_to_previous_unit() {
    if (!this->can_move_to_previous_unit()) {
        return;
    }

    this->set_unit_idx(this->_unit_idx->value() + 1);
}

bool time_editor::can_change_sign_to_plus() const {
    return true;
}

bool time_editor::can_change_sign_to_minus() const {
    return true;
}

void time_editor::change_sign_to_plus() {
    if (this->_commited_components.is_minus()) {
        this->_commited_components = this->_commited_components.is_minus_replaced(false);
        this->_components_fetcher->push();
    }
}

void time_editor::change_sign_to_minus() {
    if (!this->_commited_components.is_minus()) {
        this->_commited_components = this->_commited_components.is_minus_replaced(true);
        this->_components_fetcher->push();
    }
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

number_components time_editor::finalized_components() const {
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

observing::syncable time_editor::observe_unit_index(std::function<void(std::size_t const &)> &&handler) {
    return this->_unit_idx->observe(std::move(handler));
}

observing::syncable time_editor::observe_editing_components(std::function<void(number_components const &)> &&handler) {
    return this->_components_fetcher->observe(std::move(handler));
}

#pragma mark - private

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

void time_editor::commit_unit_value() {
    auto const editing_value = this->_editing_unit_value();
    if (!editing_value.has_value()) {
        // 編集されていないので終了
        return;
    }

    std::size_t idx = this->_unit_idx->value();
    uint32_t value = editing_value.value();

    while (idx < this->_original_components.size()) {
        if (this->_unit_idx->value() != idx) {
            // 繰り上がる値がなければ終了
            if (value == 0) {
                break;
            }
            // 繰り上がった場合は元の値に足す
            value += this->_commited_components.unit(idx).value;
        }

        auto const &original_unit = this->_original_components.unit(idx);

        if (idx == this->_original_components.size() - 1) {
            // 一番上のunitなら最大値でリミット
            uint32_t const replacing_value = std::min(value, static_cast<uint32_t>(original_unit.size - 1));
            this->_commited_components = this->_commited_components.unit_value_replaced(replacing_value, idx);
        } else {
            uint32_t const replacing_value = value % original_unit.size;
            this->_commited_components = this->_commited_components.unit_value_replaced(replacing_value, idx);
        }

        value = value / original_unit.size;

        ++idx;
    }

    this->_unit_numbers.clear();
    this->_components_fetcher->push();
}

void time_editor::_update_unit_numbers(uint32_t const value) {
    this->_unit_numbers.clear();

    auto const string = std::to_string(value);

    auto each = make_fast_each(string.length());
    while (yas_each_next(each)) {
        auto const &idx = yas_each_index(each);
        this->_unit_numbers.emplace_back(string.substr(idx, 1));
    }

    this->_components_fetcher->push();
}

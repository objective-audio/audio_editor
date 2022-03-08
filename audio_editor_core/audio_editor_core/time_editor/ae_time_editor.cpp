//
//  ae_time_editor.cpp
//

#include "ae_time_editor.h"

#include <audio_editor_core/ae_math.h>
#include <audio_editor_core/ae_time_editor_utils.h>

using namespace yas;
using namespace yas::ae;

static std::vector<std::string> const _zero_string_vector{"0"};

std::shared_ptr<time_editor> time_editor::make_shared(number_components const &components,
                                                      std::optional<std::size_t> const unit_idx) {
    return std::shared_ptr<time_editor>(new time_editor{components, unit_idx});
}

time_editor::time_editor(number_components const &components, std::optional<std::size_t> const unit_idx)
    : _state(state::editing),
      _original_components(components),
      _commited_components(time_editor_utils::to_editing_components(components)),
      _unit_idx(
          observing::value::holder<std::size_t>::make_shared(unit_idx ? unit_idx.value() : components.size() - 1)),
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

    if (this->_unit_numbers == _zero_string_vector && digits > 0) {
        return true;
    } else {
        return this->_unit_numbers.size() < digits;
    }
}

bool time_editor::can_delete_number() const {
    if (this->_is_ended()) {
        return false;
    }

    return this->_unit_numbers != _zero_string_vector;
}

bool time_editor::can_increment_number() const {
    if (this->_is_ended()) {
        return false;
    }

    auto const components = this->editing_components();
    auto const &unit = components.unit(this->_unit_idx->value());
    return unit.value < (unit.size - 1);
}

bool time_editor::can_decrement_number() const {
    if (this->_is_ended()) {
        return false;
    }

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

bool time_editor::can_set_unit_idx() const {
    return !this->_is_ended();
}

void time_editor::set_unit_idx(std::size_t const idx) {
    if (!this->can_set_unit_idx()) {
        return;
    }

    if (this->_unit_idx->value() != idx && idx < this->_original_components.size()) {
        this->_commit_unit_value();

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
    return !this->_is_ended();
}

bool time_editor::can_change_sign_to_minus() const {
    return !this->_is_ended();
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

bool time_editor::can_finish() const {
    return !this->_is_ended();
}

bool time_editor::can_cancel() const {
    return !this->_is_ended();
}

void time_editor::finish() {
    if (!this->can_finish()) {
        return;
    }

    this->_commit_unit_value();

    this->_state = state::finished;
    this->_event_notifier->notify(time_editor_event::finished);
}

void time_editor::cancel() {
    if (!this->can_cancel()) {
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

#pragma mark - responder

identifier time_editor::responder_id() {
    return this->_responder_id;
}

std::optional<ae::action> time_editor::to_action(ae::key const &key) {
    switch (key) {
        case ae::key::ret:
            return action_kind::finish_time_editing;
        case ae::key::esc:
            return action_kind::cancel_time_editing;
        case ae::key::left:
        case ae::key::shift_tab:
            return action_kind::move_to_previous_time_unit;
        case ae::key::right:
        case ae::key::tab:
            return action_kind::move_to_next_time_unit;
        case ae::key::up:
            return action_kind::increment_time;
        case ae::key::down:
            return action_kind::decrement_time;
        case ae::key::plus:
            return action_kind::change_time_sign_to_plus;
        case ae::key::hyphen:
            return action_kind::change_time_sign_to_minus;

        case ae::key::num_0:
            return action{action_kind::input_time, "0"};
        case ae::key::num_1:
            return action{action_kind::input_time, "1"};
        case ae::key::num_2:
            return action{action_kind::input_time, "2"};
        case ae::key::num_3:
            return action{action_kind::input_time, "3"};
        case ae::key::num_4:
            return action{action_kind::input_time, "4"};
        case ae::key::num_5:
            return action{action_kind::input_time, "5"};
        case ae::key::num_6:
            return action{action_kind::input_time, "6"};
        case ae::key::num_7:
            return action{action_kind::input_time, "7"};
        case ae::key::num_8:
            return action{action_kind::input_time, "8"};
        case ae::key::num_9:
            return action{action_kind::input_time, "9"};
        case ae::key::del:
            return action_kind::delete_time;

        case key::space:
        case key::a:
        case key::s:
        case key::d:
        case key::f:
        case key::n:
        case key::shift_n:
        case key::t:
        case key::left_bracket:
        case key::right_bracket:
        case key::shift_left:
        case key::shift_right:
            return std::nullopt;
    }
}

void time_editor::handle_action(ae::action const &action) {
    auto const responding = this->responding_to_action(action);
    switch (responding) {
        case responding::accepting: {
            switch (action.kind) {
                case action_kind::finish_time_editing:
                    this->finish();
                    break;
                case action_kind::cancel_time_editing:
                    this->cancel();
                    break;
                case action_kind::move_to_previous_time_unit:
                    this->move_to_previous_unit();
                    break;
                case action_kind::move_to_next_time_unit:
                    this->move_to_next_unit();
                    break;
                case action_kind::input_time:
                    this->input_number(std::stoi(action.value));
                    break;
                case action_kind::delete_time:
                    this->delete_number();
                    break;
                case action_kind::increment_time:
                    this->increment_number();
                    break;
                case action_kind::decrement_time:
                    this->decrement_number();
                    break;
                case action_kind::change_time_sign_to_plus:
                    this->change_sign_to_plus();
                    break;
                case action_kind::change_time_sign_to_minus:
                    this->change_sign_to_minus();
                    break;
                case action_kind::select_time_unit:
                    this->set_unit_idx(std::stoi(action.value));
                    break;

                case action_kind::toggle_play:
                case action_kind::nudge_previous:
                case action_kind::nudge_next:
                case action_kind::nudge_previous_more:
                case action_kind::nudge_next_more:
                case action_kind::rotate_nudging_next_unit:
                case action_kind::rotate_nudging_previous_unit:
                case action_kind::rotate_timing_fraction:
                case action_kind::jump_previous:
                case action_kind::jump_next:
                case action_kind::jump_to_beginning:
                case action_kind::jump_to_end:
                case action_kind::drop_head:
                case action_kind::split:
                case action_kind::drop_tail:
                case action_kind::erase:
                case action_kind::insert_marker:
                case action_kind::set_begin_edge:
                case action_kind::set_end_edge:
                case action_kind::return_to_zero:
                case action_kind::go_to_marker:
                case action_kind::undo:
                case action_kind::redo:
                case action_kind::select_file_for_export:
                case action_kind::cut:
                case action_kind::copy:
                case action_kind::paste:
                case action_kind::begin_time_editing:
                    break;
            }
        } break;

        case responding::blocking:
        case responding::fallthrough:
            break;
    }
}

responding time_editor::responding_to_action(ae::action const &action) {
    auto const to_responding = [](bool const &flag) { return flag ? responding::accepting : responding::blocking; };

    switch (action.kind) {
        case action_kind::finish_time_editing:
            return to_responding(this->can_finish());
        case action_kind::cancel_time_editing:
            return to_responding(this->can_cancel());
        case action_kind::move_to_previous_time_unit:
            return to_responding(this->can_move_to_previous_unit());
        case action_kind::move_to_next_time_unit:
            return to_responding(this->can_move_to_next_unit());
        case action_kind::input_time:
            return to_responding(this->can_input_number());
        case action_kind::delete_time:
            return to_responding(this->can_delete_number());
        case action_kind::increment_time:
            return to_responding(this->can_increment_number());
        case action_kind::decrement_time:
            return to_responding(this->can_decrement_number());
        case action_kind::change_time_sign_to_plus:
            return to_responding(this->can_change_sign_to_plus());
        case action_kind::change_time_sign_to_minus:
            return to_responding(this->can_change_sign_to_minus());
        case action_kind::select_time_unit:
            return to_responding(this->can_set_unit_idx());

            // 以下、project_editor用
        case action_kind::toggle_play:
        case action_kind::nudge_previous:
        case action_kind::nudge_next:
        case action_kind::nudge_previous_more:
        case action_kind::nudge_next_more:
        case action_kind::rotate_nudging_next_unit:
        case action_kind::rotate_nudging_previous_unit:
        case action_kind::rotate_timing_fraction:
        case action_kind::jump_previous:
        case action_kind::jump_next:
        case action_kind::jump_to_beginning:
        case action_kind::jump_to_end:
        case action_kind::drop_head:
        case action_kind::split:
        case action_kind::drop_tail:
        case action_kind::erase:
        case action_kind::insert_marker:
        case action_kind::return_to_zero:
        case action_kind::go_to_marker:
        case action_kind::undo:
        case action_kind::redo:
        case action_kind::select_file_for_export:
        case action_kind::cut:
        case action_kind::copy:
        case action_kind::paste:
        case action_kind::begin_time_editing:
        case action_kind::set_begin_edge:
        case action_kind::set_end_edge:
            return responding::fallthrough;
    }
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

void time_editor::_commit_unit_value() {
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

bool time_editor::_is_ended() const {
    switch (this->_state) {
        case state::finished:
        case state::canceled:
            return true;

        case state::editing:
            return false;
    }
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

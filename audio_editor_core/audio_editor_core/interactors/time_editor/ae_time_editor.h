//
//  ae_time_editor.h
//

#pragma once

#include <audio_editor_core/ae_number_components.h>
#include <audio_editor_core/ae_time_editor_types.h>
#include <cpp_utils/yas_identifier.h>
#include <observing/yas_observing_umbrella.h>

namespace yas::ae {
struct time_editor final {
    [[nodiscard]] static std::shared_ptr<time_editor> make_shared(number_components const &);

    time_editor(number_components const &);

    [[nodiscard]] bool can_input_number() const;
    [[nodiscard]] bool can_delete_number() const;
    [[nodiscard]] bool can_increment_number() const;
    [[nodiscard]] bool can_decrement_number() const;
    void input_number(uint32_t const);
    void delete_number();
    void increment_number();
    void decrement_number();

    [[nodiscard]] bool can_set_unit_idx() const;
    [[nodiscard]] bool can_move_to_next_unit() const;
    [[nodiscard]] bool can_move_to_previous_unit() const;
    void set_unit_idx(std::size_t const);
    void move_to_next_unit();
    void move_to_previous_unit();

    [[nodiscard]] bool can_change_sign_to_plus() const;
    [[nodiscard]] bool can_change_sign_to_minus() const;
    void change_sign_to_plus();
    void change_sign_to_minus();

    void commit_unit_value();

    [[nodiscard]] std::size_t unit_index() const;
    [[nodiscard]] number_components editing_components() const;
    [[nodiscard]] number_components finalized_components() const;

    [[nodiscard]] observing::syncable observe_unit_index(std::function<void(std::size_t const &)> &&);
    [[nodiscard]] observing::syncable observe_editing_components(std::function<void(number_components const &)> &&);

   private:
    number_components const _original_components;
    number_components _commited_components;
    observing::value::holder_ptr<std::size_t> const _unit_idx;
    std::vector<std::string> _unit_numbers;

    observing::fetcher_ptr<number_components> _components_fetcher;
    observing::notifier_ptr<time_editor_event> const _event_notifier;

    std::optional<uint32_t> _editing_unit_value() const;
    void _update_unit_numbers(uint32_t const);
};
}  // namespace yas::ae

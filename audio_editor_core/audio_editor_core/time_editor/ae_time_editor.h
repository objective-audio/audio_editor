//
//  ae_time_editor.h
//

#pragma once

#include <audio_editor_core/ae_number_components.h>
#include <audio_editor_core/ae_project_editor_dependency.h>
#include <audio_editor_core/ae_time_editor_types.h>
#include <audio_editor_core/ae_time_presenter_dependency.h>

namespace yas::ae {
struct time_editor final {
    [[nodiscard]] static std::shared_ptr<time_editor> make_shared(number_components const &,
                                                                  std::optional<std::size_t> const unit_idx);

    [[nodiscard]] bool can_input_number() const;
    [[nodiscard]] bool can_delete_number() const;
    [[nodiscard]] bool can_increment_number() const;
    [[nodiscard]] bool can_decrement_number() const;
    void input_number(uint32_t const);
    void delete_number();
    void increment_number();
    void decrement_number();

    [[nodiscard]] bool can_move_to_next_unit() const;
    [[nodiscard]] bool can_move_to_previous_unit() const;
    void set_unit_idx(std::size_t const);
    void move_to_next_unit();
    void move_to_previous_unit();

    [[nodiscard]] bool can_change_sign_to_plus() const;
    [[nodiscard]] bool can_change_sign_to_minus() const;
    void change_sign_to_plus();
    void change_sign_to_minus();

    [[nodiscard]] bool can_finish() const;
    [[nodiscard]] bool can_cancel() const;
    void finish();
    void cancel();

    [[nodiscard]] std::size_t unit_index() const;
    [[nodiscard]] number_components editing_components() const;
    [[nodiscard]] std::optional<number_components> finalized_components() const;

    [[nodiscard]] observing::syncable observe_unit_index(std::function<void(std::size_t const &)> &&);
    [[nodiscard]] observing::syncable observe_editing_components(std::function<void(number_components const &)> &&);
    [[nodiscard]] observing::endable observe_event(std::function<void(time_editor_event const &)> &&);

   private:
    enum class state {
        editing,
        finished,
        canceled,
    };

    state _state;
    number_components const _original_components;
    number_components _commited_components;
    observing::value::holder_ptr<std::size_t> const _unit_idx;
    std::vector<std::string> _unit_numbers;

    observing::fetcher_ptr<number_components> _components_fetcher;
    observing::notifier_ptr<time_editor_event> const _event_notifier;

    time_editor(number_components const &, std::optional<std::size_t> const unit_idx);

    std::optional<uint32_t> _editing_unit_value() const;
    void _commit_unit_value();

    bool _is_ended() const;

    void _update_unit_numbers(uint32_t const);
};
}  // namespace yas::ae

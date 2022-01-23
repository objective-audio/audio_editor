//
//  ae_time_editor.h
//

#pragma once

#include <audio_editor_core/ae_number_components.h>
#include <audio_editor_core/ae_project_editor_dependency.h>
#include <audio_editor_core/ae_time_editor_types.h>

namespace yas::ae {
struct time_editor final : time_editor_for_project_editor {
    [[nodiscard]] static std::shared_ptr<time_editor> make_shared(number_components const &);

    [[nodiscard]] bool can_input_number() const override;
    [[nodiscard]] bool can_delete_number() const override;
    void input_number(uint32_t const) override;
    void delete_number() override;

    [[nodiscard]] bool can_move_to_next_unit() const override;
    [[nodiscard]] bool can_move_to_previous_unit() const override;
    void move_to_next_unit() override;
    void move_to_previous_unit() override;

    void change_sign_to_plus() override;
    void change_sign_to_minus() override;

    void finish() override;
    void cancel() override;

    [[nodiscard]] std::size_t unit_index() const;
    [[nodiscard]] number_components editing_components() const;
    [[nodiscard]] std::optional<number_components> finalized_components() const override;

    [[nodiscard]] observing::syncable observe_unit_index(std::function<void(std::size_t const &)> &&);
    [[nodiscard]] observing::syncable observe_editing_components(std::function<void(number_components const &)> &&);
    [[nodiscard]] observing::endable observe_event(std::function<void(time_editor_event const &)> &&) override;

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

    time_editor(number_components const &);

    std::optional<uint32_t> _editing_unit_value() const;
    std::optional<uint32_t> _commited_unit_value() const;
    void _commit_unit_value();

    bool _is_ended() const;
};
}  // namespace yas::ae
